#include "lexer/token.h"

Token::Token(Type type, const std::string& text, size_t line, size_t column,
             std::optional<double> num) :
  type_{type}, text_{text}, number_{num}, line_{line}, column_{column}
{
}
