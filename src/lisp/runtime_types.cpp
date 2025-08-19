#include "lisp/runtime_types.h"

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

template<class... Ts> struct Overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

std::ostream& operator<<(std::ostream& os, const Value& value)
{
  std::visit([&](const auto& x) {
        // handle x, possibly with if constexpr on its type
      os << x;
      }, value);
  return os;
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

/*
Pair,
Primitive,
*/
