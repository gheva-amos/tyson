#ifndef TYSON_VALUE_H__
#define TYSON_VALUE_H__

#include "lisp/runtime_types.h"
class Env;
class Value
{
public:
  using Variant = std::variant<Nil, Boolean, Number, String, Symbol, List, Primitive, Lambda, Closure, Quote>;
  Value(Nil nil);
  Value(Boolean boolean);
  Value(Number number);
  Value(String string);
  Value(Symbol symbol);
  Value(List list);
  Value(Primitive primitive);
  Value(Lambda lambda);
  Value(Closure closure);
  Value(Quote quote);
  Value() {}
  Variant get_variant() const { return value_; }

  bool is_true();

  bool is_nil() const { return std::holds_alternative<Nil>(value_); }
  bool is_boolean() const { return std::holds_alternative<Boolean>(value_); }
  bool is_number() const { return std::holds_alternative<Number>(value_); }
  bool is_string() const { return std::holds_alternative<String>(value_); }
  bool is_symbol() const { return std::holds_alternative<Symbol>(value_); }
  bool is_list() const { return std::holds_alternative<List>(value_); }
  bool is_primitive() const { return std::holds_alternative<Primitive>(value_); }
  bool is_lambda() const { return std::holds_alternative<Lambda>(value_); }
  bool is_closure() const { return std::holds_alternative<Closure>(value_); }
  bool is_quote() const { return std::holds_alternative<Quote>(value_); }

  Nil& as_nil() { return std::get<Nil>(value_); }
  Boolean& as_boolean() { return std::get<Boolean>(value_); }
  Number& as_number() { return std::get<Number>(value_); }
  String& as_string() { return std::get<String>(value_); }
  Symbol& as_symbol() { return std::get<Symbol>(value_); }
  List& as_list() { return std::get<List>(value_); }
  Primitive& as_primitive() { return std::get<Primitive>(value_); }
  Lambda& as_lambda() { return std::get<Lambda>(value_); }
  Closure& as_closure() { return std::get<Closure>(value_); }
  Quote& as_quote() { return std::get<Quote>(value_); }

  Value execute(std::unique_ptr<Env>& env);
private:
  Variant value_;
};

std::ostream& operator<<(std::ostream& os, const Value& v);
#endif // TYSON_VALUE_H__
