#include "libtest.h"

#include "ggjson/ast.h"

#include <string.h>

TEST(ast, objects)
{
  struct ggjson_ast_object_field fields[3];
  fields[0].key   = "foo";
  fields[0].value = GGJSON_INTEGER(42);
  fields[1].key   = "bar";
  fields[1].value = GGJSON_INTEGER(-99);
  fields[2].key   = "baz";
  fields[2].value = GGJSON_INTEGER(101);
  ggjson_ast_object* obj = ggjson_ast_object_new(3, fields, 0);

  ASSERT_EQ(3, ggjson_ast_object_size(obj));
  ASSERT_EQ(8, ggjson_ast_object_capacity(obj));

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

TEST(ast, arrays)
{
  FAIL("implement meeeeeeeeee");
}
