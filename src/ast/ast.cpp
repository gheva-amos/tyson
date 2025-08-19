#include "ast/ast.h"
#include "lexer/token.h"
#include <stdexcept>
#include <algorithm>
#include "lisp/runtime_types.h"
#include <iostream>

AST::AST(Token& token) :
  line_{token.line()}, column_{token.column()}, type_{Type::unknown}, print_value_{token.string()}
{
}

std::ostream& AST::output(std::ostream& out) const
{
  switch (type_)
  {
  case Type::number:
    out << "Number";
    break;
  case Type::string:
    out << "String";
    break;
  case Type::boolean:
    out << "Boolean";
    break;
  case Type::nil:
    out << "Nil";
    break;
  case Type::symbol:
    out << "Symbol";
    break;
  case Type::list:
    out << "List";
    break;
  case Type::start:
    out << "Start";
    break;
  case Type::quote:
    out << "Quote";
    break;
  case Type::unknown:
    out << "Unknown";
    break;
  }
  return out;
}

void ASTStart::add_child(std::unique_ptr<AST> child)
{
  root_ = std::move(child);
}

Value ASTStart::eval(std::unique_ptr<Env>& env)
{
  return root_->eval(env);
}

ASTNumber::ASTNumber(Token& token) :
  AST{token}, value_{token.number()}
{
  type_ = AST::Type::number;
}

std::ostream& ASTNumber::output(std::ostream& out) const
{
  AST::output(out) << std::endl << value();
  return out;
}

Value ASTNumber::eval(std::unique_ptr<Env>& env)
{

  return Value{Number{value_}};
}

ASTString::ASTString(Token& token) :
  AST{token}, value_{token.string()}
{
  type_ = AST::Type::string;
}

std::ostream& ASTString::output(std::ostream& out) const
{
  AST::output(out) << std::endl << value();
  return out;
}

Value ASTString::eval(std::unique_ptr<Env>& env)
{
  return Value{String{value_}};
}

ASTBool::ASTBool(Token& token) :
  AST{token}, value_{true}
{
  type_ = AST::Type::boolean;
  std::string lc{token.string()};
  std::transform(lc.begin(), lc.end(), lc.begin(), [](char c){return std::tolower(c);});
  if (lc == "false")
  {
    value_ = false;
  }
}

std::ostream& ASTBool::output(std::ostream& out) const
{
  AST::output(out) << std::endl << (value() ? "True" : "False");
  return out;
}

Value ASTBool::eval(std::unique_ptr<Env>& env)
{
  return Value{Boolean{value_}};
}

ASTList::ASTList(Token& token) :
  AST{token}
{
  type_ = AST::Type::list;
  if (token.type() != Token::Type::open)
  {
    throw std::runtime_error("Looking for '(', found" + token.string());
  }
}

std::ostream& ASTList::output(std::ostream& out) const
{
  AST::output(out) << std::endl << '(' << std::endl << ' ';

  std::for_each(value_.begin(), value_.end(), [&out](const std::unique_ptr<AST>& ptr) { ptr->output(out) << ' '; });
  out << std::endl << ')';
  return out;
}

void ASTList::add_child(std::unique_ptr<AST> child)
{
  value_.push_back(std::move(child));
}

Value ASTList::eval(std::unique_ptr<Env>& env)
{
  List l;
  for (auto& ast : value_)
  {
    Value val{ast->eval(env)};
    l.push_back(val);
  }
  Value& first{l[0]};
  if (std::holds_alternative<Primitive>(first))
  {
    std::span<Value> args{l.begin() + 1, l.end()};
    return std::get<Primitive>(first).operator()(args);
  }
  Value ret;
  ret = l;
  return ret;
}

Value ASTList::quote(std::unique_ptr<Env>& env)
{
  List ret;
  for (auto& ast : value_)
  {
    Value v{ast->quote(env)};
    ret.push_back(v);
  }
  return Value{ret};
}

ASTSymbol::ASTSymbol(Token& token) :
  AST{token}, value_{token.string()}
{
  type_ = AST::Type::symbol;
}

Value ASTSymbol::eval(std::unique_ptr<Env>& env)
{
  auto ret{env->lookup(value_)};
  if (env->error())
  {
    throw std::runtime_error("Could not find symbol " + value_);
  }
  return ret;
}

std::ostream& ASTSymbol::output(std::ostream& out) const
{
  AST::output(out) << std::endl << value();
  return out;
}

Value ASTSymbol::quote(std::unique_ptr<Env>& env)
{
  AtomTable::Atom id{env->intern(value_)};
  return Value{Symbol{id}};
}

ASTNil::ASTNil(Token& token) :
  AST{token}, value_{token.string()}
{
  type_ = AST::Type::nil;
  std::string lc{value_};
  std::transform(lc.begin(), lc.end(), lc.begin(), [](char c){return std::tolower(c);});
  if (lc != "nil")
  {
  }
}

Value ASTNil::eval(std::unique_ptr<Env>& env)
{
  return Value{Nil{}};
}

ASTQuote::ASTQuote(Token& token) :
  AST{token}
{
  type_ = AST::Type::quote;
}

void ASTQuote::add_child(std::unique_ptr<AST> child)
{
  value_ = std::move(child);
}

Value ASTQuote::eval(std::unique_ptr<Env>& env)
{
  return value_->quote(env);
}

std::unique_ptr<AST> AST::factory(Token& token)
{
  switch (token.type())
  {
  case Token::Type::string:
    return std::make_unique<ASTString>(token);
  case Token::Type::open:
    return std::make_unique<ASTList>(token);
  case Token::Type::quote:
    return std::make_unique<ASTQuote>(token);
  case Token::Type::close:
  case Token::Type::dot:
  case Token::Type::END:
    return nullptr;
  case Token::Type::number:
    return std::make_unique<ASTNumber>(token);
  case Token::Type::symbol:
    return std::move(symbol_factory(token));
  }
  return nullptr;
}

std::unique_ptr<AST> AST::symbol_factory(Token& token)
{
  std::string value{token.string()};
  std::string lc;
  std::transform(value.begin(), value.end(), lc.begin(), [](char c){return std::tolower(c);});

  if (lc == "true" || lc == "false")
  {
    return std::make_unique<ASTBool>(token);
  }
  
  if (lc == "nil")
  {
    return std::make_unique<ASTNil>(token);
  }

  if (lc == "quote")
  {
    return std::make_unique<ASTQuote>(token);
  }

  return std::make_unique<ASTSymbol>(token);
}

