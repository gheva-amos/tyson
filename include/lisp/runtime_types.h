#ifndef TYSON_RUNTIME_TYPES_H__
#define TYSON_RUNTIME_TYPES_H__
#include <ostream>
#include <variant>
#include <vector>
#include <functional>
#include <span>
#include "lisp/atom_table.h"

class Nil;
class Boolean;
class Number;
class String;
class Symbol;
class List;
class Primitive;
using Value = std::variant<Nil, Boolean, Number, String, Symbol, List, Primitive>;

class Object
{
public:
  virtual ~Object() = default;
  virtual std::ostream& output(std::ostream& out) const = 0;
};

inline std::ostream& operator<<(std::ostream& os, const Object& o)
{
    o.output(os);
    return os;
}

class Nil : public Object
{
public:
  virtual std::ostream& output(std::ostream& out) const override;
};

class Boolean : public Object
{
public:
  virtual std::ostream& output(std::ostream& out) const override;
  Boolean& operator=(bool value);
private:
  bool value_;
};

class Number : public Object
{
public:
  virtual std::ostream& output(std::ostream& out) const override;
  Number& operator=(double value);
  Number& operator=(int value);
  bool is_int() const { return std::holds_alternative<int>(value_); }
  int as_int() const;
  double as_double() const;
private:
  std::variant<int, double> value_;
};

class String : public Object
{
public:
  virtual std::ostream& output(std::ostream& out) const override;
  String& operator=(const std::string& value);
private:
  std::string value_;
};

class Symbol : public Object
{
public:
  virtual std::ostream& output(std::ostream& out) const override;
private:
  AtomTable::Atom value_;
};

class List : public Object
{
public:
  virtual std::ostream& output(std::ostream& out) const override;
  void push_back(Value& val);
  Value& operator[](size_t index);
  std::vector<Value>::iterator begin() { return values_.begin(); }
  std::vector<Value>::iterator end() { return values_.end(); }
private:
  std::vector<Value> values_;
};

class Primitive : public Object
{
public:
  virtual std::ostream& output(std::ostream& out) const override;
  void set_name(const std::string& name);
  Value operator()(std::span<const Value> args) { return function_(args); }
  void set_function(std::function<Value(std::span<const Value>)> func);
private:
  std::string name_;
  std::function<Value(std::span<const Value>)> function_;
};

std::ostream& operator<<(std::ostream& os, const Value& v);

#endif // TYSON_RUNTIME_TYPES_H__
