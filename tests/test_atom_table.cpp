#include <gtest/gtest.h>
#include "lisp/atom_table.h"

TEST(AtomTableTests, LispTests)
{
  std::string str{"My string"};
  std::string str2{str};
  AtomTable atoms;
  AtomTable::Atom atom{atoms.intern(str)};
  EXPECT_EQ(atoms.value(atom), str);
  EXPECT_EQ(atom, 0);
  AtomTable::Atom atom2{atoms.intern(str2)};
  EXPECT_EQ(atom, atom2);
}

