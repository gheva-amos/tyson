#include "lisp/atom_table.h"

AtomTable::Atom AtomTable::intern(const std::string& atom)
{
  auto res = s_to_i_.find(atom);
  if (res != s_to_i_.end())
  {
    return res->second;
  }
  Atom ret{i_to_s_.size()};
  auto n = std::make_pair(atom, ret);
  s_to_i_.insert(n);
  i_to_s_.push_back(atom);
  return ret;
}

const std::string& AtomTable::value(Atom atom)
{
  return i_to_s_.at(atom);
}

