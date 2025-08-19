#include "lisp/frame.h"
#include <stdexcept>

Nil Frame::nil_;

Frame::Frame(AtomTable& symbols, std::shared_ptr<Frame> parent, bool is_global) :
  symbols_{symbols}, parent_{parent}, is_global_{is_global}
{
}

void Frame::define(const std::string& name, Value v)
{
  AtomTable::Atom id{symbols_.intern(name)};
  bindings_[id] = v;
}

bool Frame::set(AtomTable::Atom id, Value val)
{
  if (bindings_.find(id) == bindings_.end())
  {
    return false;
  }

  bindings_[id] = val;
  return true;
}

Value Frame::lookup(const std::string& name, bool& ret) const
{
  AtomTable::Atom id{symbols_.intern(name)};
  return lookup(id, ret);
}

Value Frame::lookup(AtomTable::Atom id, bool& ret) const
{
  auto at = bindings_.find(id);
  if (at == bindings_.end())
  {
    if (is_global_)
    {
      ret = false;
      return nil_;
    }
    return parent_->lookup(id, ret);
  }
  ret = true;
  return bindings_.at(id);
}

std::shared_ptr<Frame> Frame::parent()
{
  if (is_global_)
  {
    return nullptr;
  }
  return parent_;
}
