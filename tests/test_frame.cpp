#include <gtest/gtest.h>
#include "lisp/frame.h"

TEST(FrameConstructor, LispTests)
{
  AtomTable symbols{};
  int start{42};
  auto frame = std::make_shared<Frame>(symbols, nullptr, true);
  Number n;
  n = start;
  Value v{n};
  frame->define("A", v);
  Frame child(symbols, frame);
  bool found{false};
  auto value = child.lookup("A", found);
  EXPECT_TRUE(found);
  EXPECT_TRUE(value.is_number());
  auto num = value.as_number();
  EXPECT_TRUE(num.is_int());
  EXPECT_EQ(num.as_int(), start);
}
