#include "libtest.h"

#include "ggjson/input.h"

#include <string.h>

TEST(input, string_input) {
  const char* string = "0123456789abcdef";
  ASSERT_EQ(16, strlen(string));

  ggjson_string_input string_input;
  ggjson_string_input_init(&string_input, string, strlen(string));
 
  ggjson_input* input = (ggjson_input*)&string_input;

  for(int i = 0; i < 16; ++i)
  {
    ASSERT_EQ(0, ggjson_input_is_eof(input));
    ASSERT_EQ(i, ggjson_input_get_position(input));
    int character = ggjson_input_read_character(input);
    ASSERT_EQ(character, (int)string[i]);
  }

  ASSERT_EQ(1, ggjson_input_is_eof(input));
  ASSERT_EQ(GGJSON_EOF, ggjson_input_read_character(input));
}


