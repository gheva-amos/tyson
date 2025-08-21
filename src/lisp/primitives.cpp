#include "lisp/env.h"
#include <sstream>
#include <iostream>

void Env::load_primitives()
{
  define("+", Primitive{"ADD",
    [](std::span<Value> args) -> Value {
      double accumulator{0.0};
      for (auto& v : args)
      {
        if (v.is_number())
        {
          accumulator += v.as_number().as_double();
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
    [](std::span<Value> args) -> Value {
      double accumulator{0.0};
      bool first{true};
      for (auto& v : args)
      {
        if (first)
        {
          first = false;
          accumulator = v.as_number().as_double();
          continue;
        }
        if (v.is_number())
        {
          accumulator -= v.as_number().as_double();
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
    [](std::span<Value> args) -> Value {
      double accumulator{1.0};
      for (auto& v : args)
      {
        if (v.is_number())
        {
          accumulator *= v.as_number().as_double();
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
    [](std::span<Value> args) -> Value {
      double accumulator{args[0].as_number().as_double()};
      for (size_t i{1}; i < args.size(); ++i)
      {
        Value& v{args[i]};
        if (v.is_number())
        {
          accumulator /= v.as_number().as_double();
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
    [](std::span<Value> args) -> Value {
      List ret;
      for (auto v : args)
      {
        ret.push_back(v);
      }
      return ret;
    }
  });
  define("car", Primitive{"CAR",
    [](std::span<Value> args) -> Value {
      Value& v{args[0]};
      if (args.size() != 1)
      {
        throw std::runtime_error("car works only on a single list");
      }
      if (!v.is_list())
      {
        throw std::runtime_error("car works only on a list");
      }
      List list{v.as_list()};
      return list.car();
    }
  });
  define("cdr", Primitive{"CDR",
    [](std::span<Value> args) -> Value {
      Value& v{args[0]};
      if (args.size() != 1)
      {
        throw std::runtime_error("car works only on a single list");
      }
      if (!v.is_list())
      {
        throw std::runtime_error("car works only on a list");
      }
      List list{v.as_list()};
      return list.cdr();
    }
  });
  define("print", Primitive{"PRINT",
    [](std::span<Value> args) -> Value {
      std::stringstream ss;
      for (auto& arg : args)
      {
        ss << arg;
      }
      return Value{String{ss.str()}};
    }
  });
  define("cons", Primitive{"CONS",
    [](std::span<Value> args) -> Value {
      if (args.size() != 2)
      {
        throw std::runtime_error("cons works only on a 2 arguments");
      }
      List ret{};
      Value v{args[0]};
      ret.push_back(v);
      v = args[1];

      if (v.is_list())
      {
        List l = v.as_list();
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
    [](std::span<Value> args) -> Value {
      bool first{true};
      double current;
      for (auto& v : args)
      {
        if (!v.is_number())
        {
            throw std::runtime_error("Comparing non numbers");
        }
        if (first)
        {
          first = false;
          current = v.as_number().as_double();
          continue;
        }
        double compare_to = v.as_number().as_double();
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
    [](std::span<Value> args) -> Value {
      bool first{true};
      double current;
      for (auto& v : args)
      {
        if (!v.is_number())
        {
            throw std::runtime_error("Comparing non numbers");
        }
        if (first)
        {
          first = false;
          current = v.as_number().as_double();
          continue;
        }
        double compare_to = v.as_number().as_double();
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
    [](std::span<Value> args) -> Value {
      bool first{true};
      double current;
      for (auto& v : args)
      {
        if (!v.is_number())
        {
            throw std::runtime_error("Comparing non numbers");
        }
        if (first)
        {
          first = false;
          current = v.as_number().as_double();
          continue;
        }
        double compare_to = v.as_number().as_double();
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
