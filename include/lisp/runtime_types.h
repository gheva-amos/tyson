#ifndef TYSON_RUNTIME_TYPES_H__
#define TYSON_RUNTIME_TYPES_H__
#include <ostream>
#include <variant>
#include <vector>
#include <functional>
#include <span>
#include "lisp/atom_table.h"
#include <memory>

class Value;
class Env;

class Object
{
public:
  virtual ~Object() = default;
  virtual std::ostream& output(std::ostream& out) const = 0;
  virtual bool is_true() const { return false; }
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
  Boolean() = default;
  Boolean(bool b) : value_{b} {}
  Boolean& operator=(bool value);
  virtual bool is_true() const override { return value_; }
private:
  bool value_;
};

class Number : public Object
{
public:
  virtual std::ostream& output(std::ostream& out) const override;
  Number() = default;
  Number(double d) : value_{d} {}
  Number(int i) : value_{i} {}
  Number& operator=(double value);
  Number& operator=(int value);
  bool is_int() const { return std::holds_alternative<int>(value_); }
  int as_int() const;
  double as_double() const;
  virtual bool is_true() const override { return true; }
private:
  std::variant<int, double> value_;
};

class String : public Object
{
public:
  virtual std::ostream& output(std::ostream& out) const override;
  String() = default;
  String(const std::string& s) : value_{s} {}
  String& operator=(const std::string& value);
  virtual bool is_true() const override { return !value_.empty(); }
  const std::string& value() { return value_; }
private:
  std::string value_;
};

class Symbol : public Object
{
public:
  virtual std::ostream& output(std::ostream& out) const override;
  Symbol() = default;
  Symbol(AtomTable::Atom id) : value_{id} {}
  virtual bool is_true() const override { return true; }
  AtomTable::Atom id() { return value_; }
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
  Value car() const;
  Value cdr() const;
  Value execute(std::unique_ptr<Env>& env, bool run_lambda=false);
  virtual bool is_true() const override { return !values_.empty(); }
  size_t size() const;
private:
  std::vector<Value> values_;
};

class Primitive : public Object
{
public:
  using Function = std::function<Value(std::span<const Value>)>;
  Primitive() = default;
  Primitive(const std::string& name, Function f) : name_{name}, function_{f} {}
  virtual std::ostream& output(std::ostream& out) const override;
  void set_name(const std::string& name);
  Value operator()(std::span<const Value> args);
  void set_function(Function func);
  virtual bool is_true() const override { return true; }
private:
  std::string name_;
  Function function_;
};

class Lambda : public Object
{
public:
  virtual std::ostream& output(std::ostream& out) const override;
  Value operator()(std::span<const Value> args, std::unique_ptr<Env>& env);
  void add_statement(Value v);
  void add_arg(Value v);
private:
  std::string name_;
  std::vector<Value> statements_;
  List args_;
};

#endif // TYSON_RUNTIME_TYPES_H__
