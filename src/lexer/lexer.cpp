#include "lexer/lexer.h"
#include <sstream>
#include <cstdlib>
#include <iostream>

Lexer::Lexer(std::string src) : StringHandler(src)
{
}

bool Lexer::is_coment_start() const
{
  char c{peek()};
  if (c == ';' || c == '#')
  {
    return true;
  }
  if (c == '/' && peek(1) == '/')
  {
    return true;
  }
  return false;
}

void Lexer::skip_non_tokens()
{
  if (is_coment_start())
  {
    while (!eof() && peek() != '\n')
    {
      next();
    }
  }
  skip_space();
}

Token Lexer::token()
{
  skip_non_tokens();
  if (eof())
  {
    return {Token::Type::END, "", line(), column()};
  }
  char current{peek()};

  if (current == '(')
  {
    Token ret{Token::Type::open, "(", line(), column()};
    next();
    return ret;
  }
  if (current == ')')
  {
    Token ret{Token::Type::close, ")", line(), column()};
    next();
    return ret;
  }
  if (current == '\'')
  {
    Token ret{Token::Type::quote, "\'", line(), column()};
    next();
    return ret;
  }
  if (current == '.' && !std::isdigit(peek(1)))
  {
    Token ret{Token::Type::dot, ".", line(), column()};
    next();
    return ret;
  }

  if (current == '\"')
  {
    Token ret{get_string()};
    next();
    return ret;
  }

  if (is_number_start())
  {
    return get_number();
  }
  
  return get_symbol();
}

Token Lexer::get_string()
{
  size_t l{line()}, c{column()};
  next(); // get rid of the open parenthesis

  std::stringstream ss;
  while (!eof())
  {
    char c{peek()};
    if (c == '"')
    {
      break;
    }
    if (c == '\\')
    {
      next();
      c = peek();
    }
    ss << c;
    next();
  }

  return {Token::Type::string, ss.str(), l, c};
}

Token Lexer::get_number()
{
  size_t l{line()}, c{column()};
  std::stringstream ss;
  bool exp{false}, dot{false};
  ss << next();
  while (!eof())
  {
    if (is_space())
    {
      break;
    }
    char current{peek()};
    if (std::isdigit(current))
    {
      ss << next();
      continue;
    }
    if ((current == 'e' || current == 'E') && !exp)
    {
      exp = true;
      ss << next();
    }
    else if (current == '.' && !dot)
    {
      dot = true;
      ss << next();
    }
    else
    {
      break;
    }
  }
  char* end;
  double d = std::strtod(ss.str().c_str(), &end);
  return {Token::Type::number, "", l, c, d};
}

Token Lexer::get_symbol()
{
  size_t l{line()}, c{column()};
  std::stringstream ss;
  while (!eof())
  {
    if (is_space())
    {
      break;
    }
    ss << next();
  }
  std::string compare{ss.str()};
  if (compare == "set")
  {
    return {Token::Type::set, compare, l, c};
  }
  if (compare == "define")
  {
    return {Token::Type::define, compare, l, c};
  }
  if (compare == "nil")
  {
    return {Token::Type::nil, compare, l, c};
  }
  if (compare == "if")
  {
    return {Token::Type::if_t, compare, l, c};
  }
  if (compare == "quote")
  {
    return {Token::Type::quote, compare, l, c};
  }
  return {Token::Type::symbol, compare, l, c};
}

bool Lexer::is_number_start() const
{
  char current{peek()};

  if (std::isdigit(current))
  {
    return true;
  }

  if (current == '.' && std::isdigit(peek(1)))
  {
    return true;
  }

  if ((current == '-' || current == '+'))
  {
    char next{peek(1)};
    if (std::isdigit(next) || (next == '.' && std::isdigit(peek(2))))
    {
      return true;
    }
  }
  return false;
}


