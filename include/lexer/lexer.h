#ifndef TYSON_LEXER_H__
#define TYSON_LEXER_H__
#include "lexer/string_handler.h"
#include "lexer/token.h"
#include <vector>

class Lexer : private StringHandler
{
public:
  Lexer(std::string src);

  Token token();
  void push_back(Token token);

  const std::string& source() const { return text(); }
private:
  bool is_coment_start() const;
  void skip_non_tokens();
  Token get_string();
  Token get_number();
  Token get_symbol();
  bool is_number_start() const;
  std::vector<Token> stack_;
};

#endif // TYSON_LEXER_H__
