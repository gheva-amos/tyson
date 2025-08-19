#include "lisp/env.h"

Env::Env(std::shared_ptr<Frame> current) :
  current_{current}, had_error_{false}
{
}

Env::Env() :
  current_{nullptr}, had_error_{false}
{
  current_ = std::make_shared<Frame>(symbols_, nullptr, true);
  load_primitives();
}

Value Env::lookup(const std::string& symbol)
{
  auto id{current_->symbols().intern(symbol)};
  return lookup(id);
}

Value Env::lookup(AtomTable::Atom symbol)
{
  bool found{false};
  auto ret{current_->lookup(symbol, found)};
  if (!found)
  {
    had_error_ = true;
  }
  return ret;
}

void Env::define(const std::string& name, Value val)
{
  current_->define(name, val);
}

void Env::set(const std::string& name, Value val)
{
  auto id{current_->symbols().intern(name)};
  set(id, val);
}

void Env::set(AtomTable::Atom id, Value val)
{
  had_error_ = current_->set(id, val);
}

void Env::push()
{
  auto frame = std::make_shared<Frame>(current_->symbols(), current_);
  current_ = frame;
}

void Env::pop()
{
  auto frame = current_->parent();
  if (frame != nullptr)
  {
    current_ = frame;
  }
}

AtomTable::Atom Env::intern(const std::string& symbol)
{
  return symbols_.intern(symbol);
}

const std::string& Env::get_name(AtomTable::Atom id)
{
  return symbols_.value(id);
}

ScopedEnv::ScopedEnv(Env& env) : env_{env}
{
  env_.push();
}

ScopedEnv::~ScopedEnv()
{
  env_.pop();
}

