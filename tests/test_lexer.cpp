#include <gtest/gtest.h>
#include "lexer/lexer.h"

TEST(LexerParents, LexerTests)
{
  const std::string text{"(\"This is my text)"};

  Lexer lexer{text};
  Token t{lexer.token()};
  EXPECT_EQ(t.type(), Token::Type::open);
  EXPECT_EQ(1, t.line());
  EXPECT_EQ(1, t.column());
}

TEST(LexerParseNumber, LexerTests)
{
  const std::string text{"+345"};
  Lexer lexer{text};
  Token t{lexer.token()};
  EXPECT_EQ(t.type(), Token::Type::number);
  EXPECT_NEAR(t.number(), 345, 1e-9);
  lexer = Lexer{"345"};
  t = lexer.token();
  EXPECT_EQ(t.type(), Token::Type::number);
  EXPECT_NEAR(t.number(), 345, 1e-9);
}

TEST(LexerParseSrc, LexerTests)
{
  const std::string text{R"END((ADD 1 2))END"};
  Lexer lexer{text};
  Token token{lexer.token()};
  EXPECT_EQ(token.type(), Token::Type::open);
  token = lexer.token();
  EXPECT_EQ(token.type(), Token::Type::symbol);
  token = lexer.token();
  EXPECT_EQ(token.type(), Token::Type::number);
  EXPECT_NEAR(token.number(), 1, 1e-9);
  token = lexer.token();
  EXPECT_EQ(token.type(), Token::Type::number);
  EXPECT_NEAR(token.number(), 2, 1e-9);
  token = lexer.token();
  EXPECT_EQ(token.type(), Token::Type::close);
  token = lexer.token();
  EXPECT_EQ(token.type(), Token::Type::END);
}
