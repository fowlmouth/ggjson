#include "libtest.h"

#include "ggjson/input.h"

#include <string.h>

TEST(input, string_input) {
  const char* string = "0123456789abcdef";

  ggjson_string_input string_input;
  ggjson_string_input_init(&string_input, string, strlen(string));
 
  ggjson_input* input = (ggjson_input*)&string_input;

  ASSERT_EQ(0, ggjson_input_get_position(input));

  PASS();
}

TEST(input, fail) {
  ASSERT_EQ(0, 1);

  PASS();
}

TEST(input, fail2) {
  ASSERT_EQ(0, 1);

  PASS();
}

TEST(input, fail3) {
  ASSERT_EQ(0, 1);

  PASS();
}

TEST(input, fail4) {
  ASSERT_EQ(0, 1);

  PASS();
}

TEST(input, fail5) {
  ASSERT_EQ(0, 1);

  PASS();
}



