#include <iostream>
#include "lisp/env.h"
#include "lisp/runtime_types.h"
#include "parser/parser.h"

int main()
{
  std::unique_ptr<Env> environment = std::make_unique<Env>();
  Nil nil;
  environment->define("ADD", nil);
  environment->define("MUL", nil);
  environment->define("DIV", nil);
  std::string src{R"END(
(
  ADD
     (MUL 1 5)
     (DIV 6 3)
)
  )END"
  };
  Parser p{src};
  auto parsed{p.parse()};
  auto val(parsed->eval(environment));
  std::cout << val << std::endl;
}

