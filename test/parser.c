#include "libtest.h"

#include "ggjson/parser.h"

#include <string.h>

int test_accept_integer(ggjson_parser_state* unused, void* arg, const char* key, long long value)
{
  long long* output = arg;
  *output = value;
  return 1;
}

TEST(parser, integer)
{
  const char* string = "12345";

  ggjson_string_input string_input;
  ggjson_string_input_init(&string_input, string, strlen(string));

  ggjson_parser_events events;
  ggjson_parser_events_init(&events);
  events.accept_integer = test_accept_integer;

  long long value = 0;
  ggjson_parse(&events, (ggjson_input*)&string_input, &value);

  ASSERT_EQ(value, 12345);

  PASS();
}


