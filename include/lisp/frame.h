#ifndef TYSON_FRAME_H__
#define TYSON_FRAME_H__
#include "lisp/atom_table.h"
#include "lisp/runtime_types.h"
#include <variant>
#include <unordered_map>
#include <memory>

class Frame
{
public:
  Frame(AtomTable& symbols, std::shared_ptr<Frame> parent, bool is_global = false);
  void define(const std::string& name, Value v);
  bool set(AtomTable::Atom id, Value val);
  Value lookup(const std::string& name, bool& ret) const;
  Value lookup(AtomTable::Atom id, bool& ret) const;
  AtomTable& symbols() { return symbols_; }
  std::shared_ptr<Frame> parent();
private:
  AtomTable& symbols_;
  std::shared_ptr<Frame> parent_;
  bool is_global_;
  std::unordered_map<AtomTable::Atom, Value> bindings_;
  static Nil nil_;
};

#endif // TYSON_FRAME_H__
