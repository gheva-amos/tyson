#include <gtest/gtest.h>
#include "lexer/string_handler.h"

TEST(StringHandlerConstructor, LexerTests)
{
  const std::string text{"This is my text"};

  StringHandler handler{text};
  EXPECT_EQ(1, handler.line());
  EXPECT_EQ(1, handler.column());
}

TEST(StringHandlerPeek, LexerTests)
{
  const std::string text{"This is my text"};

  StringHandler handler{text};
  EXPECT_EQ(handler.peek(), text[0]);
  for (size_t i{0}; i < text.size(); ++i)
  {
    EXPECT_EQ(handler.peek(i), text[i]);
  }
  EXPECT_EQ(1, handler.line());
  EXPECT_EQ(1, handler.column());
}

TEST(StringHandlerNext, LexerTests)
{
  const std::string text{"line number 1\nline number 2\n\nline number 4"};

  StringHandler handler{text};
  size_t line{1}, column{1};
  for (size_t i{0}; i < text.size(); ++i)
  {
    char c{handler.next()};
    ++column;
    if (c == '\n')
    {
      ++line;
      column = 1;
    }
    EXPECT_EQ(text[i], c);
    EXPECT_EQ(handler.line(), line);
    EXPECT_EQ(handler.column(), column);
  }
}

TEST(StringHandlerSkipSpace, LexerTests)
{
  const std::string text{"     \n\n\t\nline number 1\nline number 2\n\nline number 4"};
  StringHandler handler{text};
  handler.skip_space();
  EXPECT_EQ(handler.next(), 'l');
}

