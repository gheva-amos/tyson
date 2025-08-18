#include <gtest/gtest.h>
#include "ast/ast.h"
#include "lexer/token.h"

TEST(ASTConstructors, ParserTests)
{
  double val{42};
  Token token{Token::Type::number, "", 1, 1, val};
  ASTNumber num{token};
  EXPECT_EQ(num.type(), AST::Type::number);
  EXPECT_NEAR(num.value(), val, 1e-9);

  std::string test_string{"this is my test string"};
  token = {Token::Type::string, test_string, 1, 1};
  ASTString str{token};
  EXPECT_EQ(str.type(), AST::Type::string);
  EXPECT_EQ(str.value(), test_string);

  test_string = "true";
  token = {Token::Type::symbol, test_string, 1, 1};
  ASTBool boolean{token};
  EXPECT_EQ(boolean.type(), AST::Type::boolean);
  EXPECT_TRUE(boolean.value());

  test_string = "FALSe";
  token = {Token::Type::symbol, test_string, 1, 1};
  ASTBool boolean2{token};
  EXPECT_EQ(boolean2.type(), AST::Type::boolean);
  EXPECT_FALSE(boolean2.value());

  test_string = "nil";
  token = {Token::Type::symbol, test_string, 1, 1};
  ASTNil nil{token};
  EXPECT_EQ(nil.type(), AST::Type::nil);
}

TEST(ASTFactory, ParserTests)
{
  double val{42};
  Token token{Token::Type::number, "", 1, 1, val};
  auto ast{std::move(AST::factory(token))};
  EXPECT_EQ(ast->type(), AST::Type::number);

  std::string test_string{"this is my test string"};
  token = {Token::Type::string, test_string, 1, 1};
  ast = std::move(AST::factory(token));
  EXPECT_EQ(ast->type(), AST::Type::string);
}

