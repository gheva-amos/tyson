#include "lisp/runtime_types.h"
#include "lisp/env.h"
#include "lisp/value.h"
#include <iostream>

std::ostream& Nil::output(std::ostream& out) const
{
  out << "Nil";
  return out;
}

std::ostream& Boolean::output(std::ostream& out) const
{
  out << (value_ ? "True" : "False");
  return out;
}

Boolean& Boolean::operator=(bool value)
{
  value_ = value;
  return *this;
}

std::ostream& Number::output(std::ostream& out) const
{
  if (std::holds_alternative<int>(value_))
  {
    out << std::get<int>(value_);
  }
  else
  {
    out << std::get<double>(value_);
  }
  return out;
}

Number& Number::operator=(double value)
{
  value_ = value;
  return *this;
}

Number& Number::operator=(int value)
{
  value_ = value;
  return *this;
}

int Number::as_int() const
{
  return std::get<int>(value_);
}

double Number::as_double() const
{
  return std::get<double>(value_);
}

std::ostream& String::output(std::ostream& out) const
{
  out << value_;
  return out;
}

String& String::operator=(const std::string& value)
{
  value_ = value;
  return *this;
}

std::ostream& Symbol::output(std::ostream& out) const
{
  out << value_;
  return out;
}

std::ostream& List::output(std::ostream& out) const
{
  out << '(';
  for (auto& v : values_)
  {
    out << ' ' << v;
//    v.output(out);
  }
  out << ')';
  return out;
}

void List::push_back(Value& val)
{
  values_.push_back(val);
}

Value& List::operator[](size_t index)
{
  return values_[index];
}

Value List::car() const
{
  Value v{values_[0]};
  return v;
}

Value List::cdr() const
{
  List ret{};
  for (size_t i{1}; i < values_.size(); ++i)
  {
    Value v{values_[i]};
    ret.push_back(v);
  }
  return Value{ret};
}

size_t List::size() const
{
  return values_.size();
}

Value List::execute(std::unique_ptr<Env>& env, bool run_lambda)
{
  Value& first{values_[0]};
  if (first.is_primitive())
  {
    std::span<Value> args{begin() + 1, end()};
    return first.as_primitive()(args);
  }
  if (first.is_list() && first.as_list()[0].is_lambda())
  {
    std::span<Value> args{begin() + 1, end()};
    return first.as_list()[0].as_lambda()(args, env);
  }
  return Value{Nil{}};
}

void Primitive::set_name(const std::string& name)
{
  name_ = name;
}

void Primitive::set_function(std::function<Value(std::span<const Value>)> func)
{
  function_ = func;
}

std::ostream& Primitive::output(std::ostream& out) const
{
  out << "Primitive: " << name_;
  return out;
}

Value Primitive::operator()(std::span<const Value> args)
{
  return function_(args);
}

std::ostream& Lambda::output(std::ostream& out) const
{
  out << "Lambda";
  return out;
}

void Lambda::add_statement(Value v)
{
  statements_.push_back(v);
}

Value Lambda::operator()(std::span<const Value> args, std::unique_ptr<Env>& env)
{
  if (args.size() != args_.size())
  {
    throw std::runtime_error("wrong number of arguments passed to lambda");
  }
  env->push();
  for (size_t i{0}; i < args.size(); ++i)
  {
    env->define(args_[i].as_string().value(), args[i]);
  }
  for (auto s : statements_)
  {
    if (s.is_list())
    {
      List statement = s.as_list();
      statement.execute(env, true);
    }
  }
  env->pop();
  return String{"jhel"};
}

void Lambda::add_arg(Value v)
{
  if (!v.is_list())
  {
    throw std::runtime_error("calling lambda without an argument list");
  }
  args_ = v.as_list();
}

