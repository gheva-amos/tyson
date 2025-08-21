#include <iostream>
#include "lisp/env.h"
#include "lisp/runtime_types.h"
#include "parser/parser.h"

int main()
{
  std::unique_ptr<Env> environment = std::make_unique<Env>();
  std::string src3{R"END(
(quote (
list 6 (* 1 3)
))
  )END" // should return 2
  };
  std::string src2{R"END(
(
/ 6 (* 1 3)
)
  )END" // should return 2
  };
  std::string src{R"END(
(
  +
     (* 2 5)
     (/ 6 3)
)
  )END" // should return 12
  };
  Parser p{src3};
  auto parsed{p.parse()};
  auto val(parsed->eval(environment));
  std::cout << val << std::endl;
}

