#include "lisp/env.h"
#include <sstream>
#include <iostream>

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
          continue;
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
  define("car", Primitive{"CAR",
    [](std::span<const Value> args) -> Value {
      const Value& v{args[0]};
      if (args.size() != 1)
      {
        throw std::runtime_error("car works only on a single list");
      }
      if (!std::holds_alternative<List>(v))
      {
        throw std::runtime_error("car works only on a list");
      }
      List list{std::get<List>(v)};
      return list.car();
    }
  });
  define("cdr", Primitive{"CDR",
    [](std::span<const Value> args) -> Value {
      const Value& v{args[0]};
      if (args.size() != 1)
      {
        throw std::runtime_error("car works only on a single list");
      }
      if (!std::holds_alternative<List>(v))
      {
        throw std::runtime_error("car works only on a list");
      }
      List list{std::get<List>(v)};
      return list.cdr();
    }
  });
  define("print", Primitive{"PRINT",
    [](std::span<const Value> args) -> Value {
      std::stringstream ss;
      for (auto& arg : args)
      {
        ss << arg;
      }
      return Value{String{ss.str()}};
    }
  });
  define("cons", Primitive{"CONS",
    [](std::span<const Value> args) -> Value {
      if (args.size() != 2)
      {
        throw std::runtime_error("cons works only on a 2 arguments");
      }
      List ret{};
      Value v{args[0]};
      ret.push_back(v);
      v = args[1];

      if (std::holds_alternative<List>(v))
      {
        List l = std::get<List>(v);
        for (auto& c : l)
        {
          ret.push_back(c);
        }
      }
      else
      {
        ret.push_back(v);
      }
      return Value{ret};
    }
  });
  define("<", Primitive{"LT",
    [](std::span<const Value> args) -> Value {
      bool first{true};
      double current;
      for (auto& v : args)
      {
        if (!std::holds_alternative<Number>(v))
        {
            throw std::runtime_error("Comparing non numbers");
        }
        if (first)
        {
          first = false;
          current = std::get<Number>(v).as_double();
          continue;
        }
        double compare_to = std::get<Number>(v).as_double();
        if (current >= compare_to)
        {
          return Value{Boolean{false}};
        }
        current = compare_to;
      }
      return Value{Boolean{true}};
    }
  });
  define(">", Primitive{"GT",
    [](std::span<const Value> args) -> Value {
      bool first{true};
      double current;
      for (auto& v : args)
      {
        if (!std::holds_alternative<Number>(v))
        {
            throw std::runtime_error("Comparing non numbers");
        }
        if (first)
        {
          first = false;
          current = std::get<Number>(v).as_double();
          continue;
        }
        double compare_to = std::get<Number>(v).as_double();
        if (current <= compare_to)
        {
          return Value{Boolean{false}};
        }
        current = compare_to;
      }
      return Value{Boolean{true}};
    }
  });
  define("=", Primitive{"EQ",
    [](std::span<const Value> args) -> Value {
      bool first{true};
      double current;
      for (auto& v : args)
      {
        if (!std::holds_alternative<Number>(v))
        {
            throw std::runtime_error("Comparing non numbers");
        }
        if (first)
        {
          first = false;
          current = std::get<Number>(v).as_double();
          continue;
        }
        double compare_to = std::get<Number>(v).as_double();
        if (current != compare_to)
        {
          return Value{Boolean{false}};
        }
        current = compare_to;
      }
      return Value{Boolean{true}};
    }
  });
}
