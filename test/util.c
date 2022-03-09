#include "libtest.h"
#include "ggjson/util.h"

TEST(util, next_power_of_two)
{
  ASSERT_EQ(2, next_power_of_two(2));

  ASSERT_EQ(4, next_power_of_two(3));
  ASSERT_EQ(4, next_power_of_two(4));

  ASSERT_EQ(8, next_power_of_two(5));
  ASSERT_EQ(8, next_power_of_two(6));
  ASSERT_EQ(8, next_power_of_two(7));
  ASSERT_EQ(8, next_power_of_two(8));

  PASS();
}