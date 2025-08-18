#include "lexer/string_handler.h"
#include <utility>
#include <cctype>

StringHandler::StringHandler(std::string text) :
  text_{std::move(text)}, index_{0}, line_{1}, column_{1}
{
}



bool StringHandler::eof(size_t forward) const
{
  return index_ + forward >= text_.size();
}

char StringHandler::peek(size_t forward) const
{
  if (eof(forward))
  {
    return '\0';
  }
  return text_[index_ + forward];
}

char StringHandler::next()
{
  char ret{peek()};
  ++index_;
  ++column_;
  if (ret == '\n')
  {
    ++line_;
    column_ = 1;
  }
  return ret;
}

bool StringHandler::is_space() const
{
  char c{peek()};
  return std::isspace(c);
}

void StringHandler::skip_space()
{
  while (!eof())
  {
    if (is_space())
    {
      next();
      continue;
    }
    break;
  }
}

