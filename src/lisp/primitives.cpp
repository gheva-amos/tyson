#include "lisp/env.h"

void Env::load_primitives()
{
  define("+", Primitive{"ADD",
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
  define("-", Primitive{"SUB",
    [](std::span<const Value> args) -> Value {
      double accumulator{0.0};
      bool first{true};
      for (auto& v : args)
      {
        if (first)
        {
          first = false;
          accumulator = std::get<Number>(v).as_double();
        }
        if (std::holds_alternative<Number>(v))
        {
          accumulator -= std::get<Number>(v).as_double();
        }
        else
        {
          throw std::runtime_error("trying to subtract not a number");
        }
      }
      if (args.size() == 1)
      {
        return Value{Number{-accumulator}};
      }
      return Value{Number{accumulator}};
    }
  });
  define("*", Primitive{"NUL",
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
          throw std::runtime_error("trying to multiply not a number");
        }
      }
      return Value{Number{accumulator}};
    }
  });
  define("/", Primitive{"DIV",
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
          throw std::runtime_error("trying to devide not a number");
        }
      }
      return Value{Number{accumulator}};
    }
  });
  define("list", Primitive{"LIST",
    [](std::span<const Value> args) -> Value {
      List ret;
      for (auto v : args)
      {
        ret.push_back(v);
      }
      return ret;
    }
  });
}
