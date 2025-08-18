#ifndef TYSON_PARSER_H__
#define TYSON_PARSER_H__
#include <string>
#include "lexer/lexer.h"
#include "ast/ast.h"
#include <memory>
#include <vector>

class Parser
{
public:
  Parser(std::string src);
  Parser(Lexer& lexer);
  std::unique_ptr<AST> parse();
private:
  Token token() { return lexer_.token(); }
  Lexer lexer_;

  void parse_form(std::vector<AST*>& stack);
};

#endif // TYSON_PARSER_H__
