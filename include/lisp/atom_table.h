#ifndef TYSON_ATOM_TABLE_H__
#define TYSON_ATOM_TABLE_H__

#include <unordered_map>
#include <vector>
#include <string>

class AtomTable
{
public:
  using Atom = size_t;
  Atom intern(const std::string& atom);
  const std::string& value(Atom atom);
private:
  std::unordered_map<std::string, Atom> s_to_i_;
  std::vector<std::string> i_to_s_;
};

#endif // TYSON_ATOM_TABLE_H__
