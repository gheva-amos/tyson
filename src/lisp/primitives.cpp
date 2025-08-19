#include "lisp/env.h"

void Env::load_primitives()
{
  define("ADD", Primitive{"ADD",
    [](std::span<const Value> args) -> Value {
      double accumulator{0.0};
      for (auto& v : args)
      {
        if (std::holds_alternative<Number>(v))
        {
          accumulator += std::get<Number>(v).as_double();
        }
        else
        {
          throw std::runtime_error("trying to add not a number");
        }
      }
      return Value{Number{accumulator}};
    }
  });
  define("MUL", Primitive{"NUL",
    [](std::span<const Value> args) -> Value {
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
      return Value{Number{accumulator}};
    }
  });
  define("DIV", Primitive{"DIV",
    [](std::span<const Value> args) -> Value {
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
      return Value{Number{accumulator}};
    }
  });
}
