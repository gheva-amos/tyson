#include "lisp/runtime_types.h"
#include "lisp/env.h"
#include "lisp/value.h"
#include <iostream>

Value Object::execute(std::unique_ptr<Env>& env)
{
  Nil nil;
  return nil;
}

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

Value Boolean::execute(std::unique_ptr<Env>& env)
{
  Boolean b{value_};
  return b;
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

Value Number::execute(std::unique_ptr<Env>& env)
{
  Number n{as_double()};
  return n;
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

Value String::execute(std::unique_ptr<Env>& env)
{
  String s{value_};
  return s;
}

std::ostream& Symbol::output(std::ostream& out) const
{
  out << value_ << ' ' << name_;
  return out;
}

Value Symbol::execute(std::unique_ptr<Env>& env)
{
  return env->lookup(value_);
}

std::ostream& List::output(std::ostream& out) const
{
  out << '(';
  for (auto& v : values_)
  {
    out << ' ' << v;
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

Value List::execute(std::unique_ptr<Env>& env)
{
  Value first{values_[0].execute(env)};
  if (first.is_list())
  {
    return first.execute(env);
  }
  if (first.is_symbol())
  {
    return first.execute(env);
  }
  if (first.is_primitive())
  {
    std::span<Value> args{begin() + 1, end()};
    std::vector<Value> arguments{};
    for (auto arg : args)
    {
      arguments.push_back(arg.execute(env));
    }
    return first.as_primitive()(arguments);
  }
  if (first.is_lambda())
  {
    return first.execute(env);
  }
  if (first.is_closure())
  {
    std::span<Value> args{begin() + 1, end()};
    std::vector<Value> arguments{};
    for (auto arg : args)
    {
      arguments.push_back(arg.execute(env));
    }
    return first.as_closure()(arguments, env);
  }
  Nil nil{};
  return Value{nil};
}

void Primitive::set_name(const std::string& name)
{
  name_ = name;
}

void Primitive::set_function(Function func)
{
  function_ = func;
}

std::ostream& Primitive::output(std::ostream& out) const
{
  out << "Primitive: " << name_;
  return out;
}

Value Primitive::operator()(std::span<Value> args)
{
  return function_(args);
}

Value Primitive::execute(std::unique_ptr<Env>& env)
{
  return *this;
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

Value Lambda::operator()(std::span<Value> args, std::unique_ptr<Env>& env)
{
  Value ret{Nil{}};
  if (args.size() != args_.size())
  {
    throw std::runtime_error("wrong number of arguments passed to lambda");
  }
  env->push();
  for (size_t i{0}; i < args.size(); ++i)
  {
    env->define(args_[i].as_symbol().value(), args[i]);
  }
  for (auto s : statements_)
  {
    if (s.is_list())
    {
      List statement = s.as_list();
      ret = statement.execute(env);
    }
  }
  env->pop();
  env->pop();
  return ret;
}

Value Lambda::execute(std::unique_ptr<Env>& env)
{
  Closure ret{};
  ret.set_lambda(*this);
  return ret.execute(env);
}

void Lambda::add_arg(Value v)
{
  if (!v.is_list())
  {
    throw std::runtime_error("calling lambda without an argument list");
  }
  args_ = v.as_list();
}

std::ostream& Closure::output(std::ostream& out) const
{
  out << "Closure";
  return out;
}

Value Closure::execute(std::unique_ptr<Env>& env)
{
  frame_ = env->get_frame();
  return *this;
}

Value Closure::operator()(std::span<Value> args, std::unique_ptr<Env>& env)
{
  env->add_frame(frame_);
  return lambda_(args, env);
}

void Closure::set_lambda(Lambda l)
{
  lambda_ = l;
}

std::ostream& Quote::output(std::ostream& out) const
{
  out << "quote";
  return out;
}

Value Quote::execute(std::unique_ptr<Env>& env)
{
  return value_[0];
}

void Quote::add_value(Value v)
{
  value_.push_back(v);
}

