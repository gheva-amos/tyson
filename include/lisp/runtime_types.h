#ifndef TYSON_RUNTIME_TYPES_H__
#define TYSON_RUNTIME_TYPES_H__
#include <ostream>
#include <variant>
#include "lisp/atom_table.h"

class Object
{
public:
  virtual ~Object() = default;
  virtual std::ostream& output(std::ostream& out) const = 0;
};

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

#endif // TYSON_RUNTIME_TYPES_H__
