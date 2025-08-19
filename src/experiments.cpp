#include <iostream>
#include "lisp/env.h"
#include "lisp/runtime_types.h"
#include "parser/parser.h"

int main()
{
  std::unique_ptr<Env> environment = std::make_unique<Env>();
  Primitive add, mul, div;
  add.set_name("ADD");
  add.set_function([](std::span<const Value> args) -> Value {
    double accumulator{0.0};
    for (auto& v : args)
    {
    std::cout << std::get<Number>(v) << std::endl;
      if (std::holds_alternative<Number>(v))
      {
        accumulator += std::get<Number>(v).as_double();
      }
      else
      {
        throw std::runtime_error("trying to add not a number");
      }
    }
    Number n;
    n = accumulator;
    Value ret;
    ret = n;
    return ret;
  });
  environment->define("ADD", add);
  mul.set_name("MUL");
  mul.set_function([](std::span<const Value> args) -> Value {
    double accumulator{1.0};
    for (auto& v : args)
    {
      if (std::holds_alternative<Number>(v))
      {
        accumulator *= std::get<Number>(v).as_double();
      }
      else
      {
        throw std::runtime_error("trying to add not a number");
      }
    }
    Number n;
    n = accumulator;
    Value ret;
    ret = n;
    return ret;
  });
  environment->define("MUL", mul);
  div.set_function([](std::span<const Value> args) -> Value {
    double accumulator{std::get<Number>(args[0]).as_double()};
    for (size_t i{1}; i < args.size(); ++i)
    {
      const Value& v{args[i]};
      if (std::holds_alternative<Number>(v))
      {
        accumulator /= std::get<Number>(v).as_double();
      }
      else
      {
        throw std::runtime_error("trying to add not a number");
      }
    }
    Number n;
    n = accumulator;
    Value ret;
    ret = n;
    return ret;
  });
  div.set_name("DIV");
  environment->define("DIV", div);
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

