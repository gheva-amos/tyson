#include "parser/parser.h"
#include <utility>
#include <vector>
#include <iostream>

Parser::Parser(std::string src) : lexer_{std::move(src)}
{
}

Parser::Parser(Lexer& lexer) : lexer_{lexer.source()}
{
}

std::unique_ptr<AST> Parser::parse()
{
  Token t{Token::Type::symbol, "BEGIN", 0, 0};
  std::unique_ptr<AST> ret = std::make_unique<ASTStart>(t);
  std::vector<AST*> stack;
  stack.push_back(ret.get());
  parse_form(stack);
  return ret;
}

void Parser::parse_form(std::vector<AST*>& stack)
{
  Token current = token();
  if (current.type() == Token::Type::END)
  {
    if (stack.size() != 1)
    {
    }
    return;
  }
  std::unique_ptr<AST> next{nullptr};
  
  switch (current.type())
  {
  case Token::Type::let:
    next = std::make_unique<ASTLet>(current);
    break;
  case Token::Type::set:
    next = std::make_unique<ASTSet>(current);
    break;
  case Token::Type::define:
    next = std::make_unique<ASTDefine>(current);
    break;
  case Token::Type::if_t:
    next = std::make_unique<ASTIf>(current);
    break;
  case Token::Type::quote:
    next = std::make_unique<ASTQuote>(current);
    break;
  case Token::Type::open:
    next = std::make_unique<ASTList>(current);
    break;
  case Token::Type::close:
    stack.pop_back();
    break;
  default:
    stack.back()->add_child(std::move(AST::factory(current)));
    break;
  }
  if (next != nullptr)
  {
    AST* root = stack.back();
    stack.push_back(next.get());
    root->add_child(std::move(next));
  }
  parse_form(stack);
}

