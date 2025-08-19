#include "parser/parser.h"
#include <utility>
#include <vector>

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
  if (current.type() == Token::Type::quote)
  {
    std::unique_ptr<AST> quote{std::make_unique<ASTQuote>(current)};
    AST* root = stack.back();
    stack.push_back(quote.get());
    root->add_child(std::move(quote));
    parse_form(stack);
    return;
  }
  if (current.type() == Token::Type::open)
  {
    std::unique_ptr<AST> list{std::make_unique<ASTList>(current)};
    AST* root = stack.back();
    stack.push_back(list.get());
    root->add_child(std::move(list));
    parse_form(stack);
    return;
  }
  if (current.type() == Token::Type::close)
  {
   // TODO if empty list retun nil
    stack.pop_back();
    parse_form(stack);
    return;
  }
  stack.back()->add_child(std::move(AST::factory(current)));
  parse_form(stack);
}

