#include <gtest/gtest.h>
#include "parser/parser.h"
#include <iostream>

TEST(ParserConstructor, ParserTests)
{
  std::string code{R"END(
((ADD
  (MUL 1 2)
  (DIV 4 2)
)))END"};
  Parser parser{code};
  std::unique_ptr<AST> parsed{std::move(parser.parse())};
  EXPECT_EQ(parsed->type(), AST::Type::start);
  //std::cout << parsed->get_child()->str() << std::endl;
  EXPECT_EQ(parsed->get_child()->type(), AST::Type::list);
  //parsed->get_child()->output(std::cout);
}

