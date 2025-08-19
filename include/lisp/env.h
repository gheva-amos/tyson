#ifndef TYSON_ENV_H__
#define TYSON_ENV_H__
#include <memory>
#include <string>
#include "lisp/frame.h"
#include "lisp/runtime_types.h"

class Env
{
public:
  Env(std::shared_ptr<Frame> current);
  Env();
  Value lookup(const std::string& symbol);
  Value lookup(AtomTable::Atom symbol);
  bool error() const { return had_error_; }
  void define(const std::string& name, Value val);
  void set(const std::string& name, Value val);
  void set(AtomTable::Atom id, Value val);
  void push();
  void pop();
private:
  AtomTable symbols_{};
  std::shared_ptr<Frame> current_;
  bool had_error_;
};

class ScopedEnv
{
public:
  ScopedEnv(Env& env);
  ~ScopedEnv();
private:
  Env& env_;
};
#endif // TYSON_ENV_H__
