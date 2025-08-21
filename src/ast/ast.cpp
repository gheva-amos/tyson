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
  case Type::if_t:
    out << "If";
    break;
  case Type::define:
    out << "Define";
    break;
  case Type::set:
    out << "Set";
    break;
  case Type::let:
    out << "Let";
    break;
  case Type::lambda:
    out << "Lambda";
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

Value ASTStart::quote(std::unique_ptr<Env>& env)
{
  return root_->quote(env);
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
  //l.execute(env);
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
  return Value{Symbol{id, value_}};
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
  Quote ret;
  ret.add_value(value_->quote(env));
  return ret;
}

ASTIf::ASTIf(Token& token) :
  AST{token}, count_{0}
{
  type_ = AST::Type::if_t;
}

Value ASTIf::eval(std::unique_ptr<Env>& env)
{
  Value test{test_->eval(env)};

  if (test.is_true())
  {
    return true_->eval(env);
  }
  return else_->eval(env);
}

Value ASTIf::quote(std::unique_ptr<Env>& env)
{
  List ret;
  Value tmp{String{"if"}};
  ret.push_back(tmp);
  tmp = test_->quote(env);
  ret.push_back(tmp);
  tmp = true_->quote(env);
  ret.push_back(tmp);
  tmp = else_->quote(env);
  ret.push_back(tmp);
  return Value{ret};
}

void ASTIf::add_child(std::unique_ptr<AST> child)
{
  if (count_ >= 3)
  {
    throw std::runtime_error("too many arguments for if");
  }
  switch (count_)
  {
  case 0:
    test_ = std::move(child);
    break;
  case 1:
    true_ = std::move(child);
    break;
  case 2:
    else_ = std::move(child);
    break;
  }
  ++count_;
}

ASTDefine::ASTDefine(Token& token) :
  AST{token}, count_{0}
{
  type_ = AST::Type::define;
}

void ASTDefine::add_child(std::unique_ptr<AST> child)
{
  if (count_ >= 2)
  {
    throw std::runtime_error("too many arguments for define");
  }
  switch(count_)
  {
  case 0:
    symbol_ = std::move(child);
    break;
  case 1:
    value_ = std::move(child);
    break;
  }
  ++count_;
}

Value ASTDefine::eval(std::unique_ptr<Env>& env)
{
  Value ret = value_->eval(env).execute(env);
  env->define(symbol_->as_string(), ret);
  return ret;
}

Value ASTDefine::quote(std::unique_ptr<Env>& env)
{
  List ret;
  Value tmp{String{"define"}};
  ret.push_back(tmp);
  tmp = symbol_->quote(env);
  ret.push_back(tmp);
  tmp = value_->quote(env);
  ret.push_back(tmp);
  return Value{ret};
}

ASTSet::ASTSet(Token& token) :
  AST{token}, count_{0}
{
  type_ = AST::Type::set;
}

Value ASTSet::eval(std::unique_ptr<Env>& env)
{
  Value ret = value_->eval(env);
  env->set(symbol_->as_string(), ret);
  if (env->error())
  {
    throw std::runtime_error("Error trying to set " + symbol_->as_string());
  }
  return ret;
}

Value ASTSet::quote(std::unique_ptr<Env>& env)
{
  List ret;
  Value tmp{String{"set"}};
  ret.push_back(tmp);
  tmp = symbol_->quote(env);
  ret.push_back(tmp);
  tmp = value_->quote(env);
  ret.push_back(tmp);
  return Value{ret};
}

void ASTSet::add_child(std::unique_ptr<AST> child)
{
  if (count_ >= 2)
  {
    throw std::runtime_error("too many arguments for set");
  }
  switch(count_)
  {
  case 0:
    symbol_ = std::move(child);
    break;
  case 1:
    value_ = std::move(child);
    break;
  }
  ++count_;
}

ASTLet::ASTLet(Token& token) :
  AST{token}, bindings_{nullptr}
{
  type_ = AST::Type::let;
}

Value ASTLet::eval(std::unique_ptr<Env>& env)
{
  ASTList* lst = dynamic_cast<ASTList*>(bindings_.get());
  
  std::vector<Value> values{};
  for (size_t i{0}; i < lst->size(); ++i)
  {
    ASTList* ast = dynamic_cast<ASTList*>(lst->get_child_at(i));
    if (ast->size() != 2)
    {
      throw std::runtime_error("Let with a wrong shape of binding");
    }
    values.push_back(ast->get_child_at(1)->eval(env));
  }
  env->push();
  for (size_t i{0}; i < values.size(); ++i)
  {
    ASTList* ast = dynamic_cast<ASTList*>(lst->get_child_at(i));
    env->define(ast->get_child_at(0)->as_string(), values[i]); 
  }
  Value ret;
  for (auto& ast : statements_)
  {
    ret = ast->eval(env);
  }
  env->pop();
  return ret;
}

Value ASTLet::quote(std::unique_ptr<Env>& env)
{
  List ret;
  Value tmp{String{"let"}};
  ret.push_back(tmp);
  tmp = bindings_->quote(env);
  ret.push_back(tmp);
  for (auto& statement : statements_)
  {
    tmp = statement->quote(env);
    ret.push_back(tmp);
  }
  return Value{ret};
}

void ASTLet::add_child(std::unique_ptr<AST> child)
{
  if (bindings_ == nullptr)
  {
    bindings_ = std::move(child);
  }
  else
  {
    statements_.push_back(std::move(child));
  }
}

ASTLambda::ASTLambda(Token& token) :
  AST{token}, bindings_{nullptr}
{
  type_ = AST::Type::lambda;
}

Value ASTLambda::eval(std::unique_ptr<Env>& env)
{
  Lambda l;
  for (auto& statement : statements_)
  {
    auto tmp = statement->quote(env);
    l.add_statement(tmp);
  }
  if (bindings_ == nullptr)
  {
    throw std::runtime_error("No bindings for lambda");
  }
  l.add_arg(bindings_->quote(env));
  return Value{l}.execute(env);
}

Value ASTLambda::quote(std::unique_ptr<Env>& env)
{
  List ret;
  Value tmp{String{"lambda"}};
  ret.push_back(tmp);
  tmp = bindings_->quote(env);
  ret.push_back(tmp);
  for (auto& statement : statements_)
  {
    tmp = statement->quote(env);
    ret.push_back(tmp);
  }
  return Value{ret};
}

void ASTLambda::add_child(std::unique_ptr<AST> child)
{
  if (bindings_ == nullptr)
  {
    bindings_ = std::move(child);
  }
  else
  {
    statements_.push_back(std::move(child));
  }
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
  case Token::Type::if_t:
    return std::make_unique<ASTIf>(token);
  case Token::Type::nil:
    return std::make_unique<ASTNil>(token);
  case Token::Type::define:
    return std::make_unique<ASTDefine>(token);
  case Token::Type::set:
    return std::make_unique<ASTSet>(token);
  case Token::Type::let:
    return std::make_unique<ASTLet>(token);
  case Token::Type::lambda:
    return std::make_unique<ASTLambda>(token);
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
  
  if (lc == "set")
  {
    return std::make_unique<ASTSet>(token);
  }

  if (lc == "define")
  {
    return std::make_unique<ASTDefine>(token);
  }

  if (lc == "nil")
  {
    return std::make_unique<ASTNil>(token);
  }

  if (lc == "quote")
  {
    return std::make_unique<ASTQuote>(token);
  }

  if (lc == "if")
  {
    return std::make_unique<ASTIf>(token);
  }

  if (lc == "let")
  {
    return std::make_unique<ASTLet>(token);
  }

  if (lc == "lambda")
  {
    return std::make_unique<ASTLambda>(token);
  }

  return std::make_unique<ASTSymbol>(token);
}

