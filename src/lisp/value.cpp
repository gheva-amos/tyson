#include "lisp/value.h"

Value::Value(Nil nil)
{
  value_ = nil;
}

Value::Value(Boolean boolean)
{
  value_ = boolean;
}

Value::Value(Number number)
{
  value_ = number;
}

Value::Value(String string)
{
  value_ = string;
}

Value::Value(Symbol symbol)
{
  value_ = symbol;
}

Value::Value(List list)
{
  value_ = list;
}

Value::Value(Primitive primitive)
{
  value_ = primitive;
}

template<class... Ts> struct Overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

std::ostream& operator<<(std::ostream& os, const Value& value)
{
  std::visit([&](const auto& x) {
        // handle x, possibly with if constexpr on its type
      os << x;
      }, value.get_variant());
  return os;
}

bool Value::is_true()
{
  return std::visit([](auto const& x) -> bool { return x.is_true(); }, value_);
}
