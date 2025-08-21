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
  
  if (current.type() == Token::Type::open)
  {
    Token next_token = token();
    switch (next_token.type())
    {
    case Token::Type::lambda:
      next = std::make_unique<ASTLambda>(next_token);
      break;
    case Token::Type::let:
      next = std::make_unique<ASTLet>(next_token);
      break;
    case Token::Type::set:
      next = std::make_unique<ASTSet>(next_token);
      break;
    case Token::Type::define:
      next = std::make_unique<ASTDefine>(next_token);
      break;
    case Token::Type::if_t:
      next = std::make_unique<ASTIf>(next_token);
      break;
    case Token::Type::quote:
      next = std::make_unique<ASTQuote>(next_token);
      break;
    case Token::Type::open:
      {
        push_back(next_token);
        next = std::make_unique<ASTList>(current); // TODO
      }
      break;
    case Token::Type::close:
      {
        AST* root = stack.back();
        root->add_child(std::make_unique<ASTNil>(next_token));
      }
      break;
    default:
      {
        next = std::make_unique<ASTList>(current); // TODO
        AST* root = stack.back();
        stack.push_back(next.get());
        root->add_child(std::move(next));
        next = nullptr;
        stack.back()->add_child(std::move(AST::factory(next_token))); // TODO
      }
      break;
    }
    if (next != nullptr)
    {
      AST* root = stack.back();
      stack.push_back(next.get());
      root->add_child(std::move(next));
    }
  }
  else if (current.type() == Token::Type::close)
  {
    stack.pop_back();
  }
  else
  {
    stack.back()->add_child(std::move(AST::factory(current)));
  }
  parse_form(stack);
}

