#include <iostream>
#include "lisp/env.h"
#include "lisp/runtime_types.h"
#include "parser/parser.h"

int main()
{
  std::unique_ptr<Env> environment = std::make_unique<Env>();
  std::string src2{R"END(
(
DIV 6 (MUL 1 3)
)
  )END" // should return 2
  };
  std::string src{R"END(
(
  ADD
     (MUL 2 5)
     (DIV 6 3)
)
  )END" // should return 12
  };
  Parser p{src};
  auto parsed{p.parse()};
  auto val(parsed->eval(environment));
  std::cout << val << std::endl;
}

