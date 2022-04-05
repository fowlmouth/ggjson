#include "libtest.h"

#include "ggjson/parser.h"

#include <string.h>

struct parser_test_state
{
  ggjson_parser parser;
};

void parser_test_setup(void* arg)
{
  struct parser_test_state* state = arg;
  memset(&state->parser, 0, sizeof(ggjson_parser));
}

void parser_test_destroy(void* arg)
{
  struct parser_test_state* state = arg;
  ggjson_parser_deinit(&state->parser);
}

SUITE(parser, sizeof(struct parser_test_state), parser_test_setup, parser_test_destroy)





int test_accept_integer(ggjson_parser_state* unused, void* arg, const char* key, long long value)
{
  long long* output = arg;
  *output = value;
  return 1;
}

TEST(parser, integer)
{
  const char* string = "12345 -99 42";

  ggjson_string_input string_input;
  ggjson_string_input_init(&string_input, string, strlen(string));

  ggjson_parser_events events;
  ggjson_parser_events_init(&events);
  events.accept_integer = test_accept_integer;

  ggjson_parser* parser = &((struct parser_test_state*)data)->parser;
  ggjson_parser_init(parser, &events, (ggjson_input*)&string_input);

  long long value = 0;

  ASSERT(ggjson_parse_node(parser, &value));
  ASSERT_EQ(value, 12345);

  ASSERT(ggjson_parse_node(parser, &value));
  ASSERT_EQ(value, -99);

  ASSERT(ggjson_parse_node(parser, &value));
  ASSERT_EQ(value, 42);

  ASSERT(ggjson_input_is_eof((ggjson_input*)&string_input));
  ASSERT(!ggjson_parse_node(parser, &value));
}

struct string_buffer
{
  int size;
  char* buffer;
};

int test_accept_string(ggjson_parser_state* unused, void* arg, const char* key, int size, const char* str)
{
  struct string_buffer* output = arg;
  snprintf(output->buffer, output->size, "%s", str);
  return 1;
}

TEST(parser, string)
{
  const char* string = "\"A🍄B🌶C🍍\" \"foo\nbar\"";

  ggjson_string_input string_input;
  ggjson_string_input_init(&string_input, string, strlen(string));

  ggjson_parser_events events;
  ggjson_parser_events_init(&events);
  events.accept_string = test_accept_string;

  ggjson_parser* parser = &((struct parser_test_state*)data)->parser;
  ggjson_parser_init(parser, &events, (ggjson_input*)&string_input);

  char output[64];
  struct string_buffer strbuf = {64, output};

  ASSERT(ggjson_parse_node(parser, &strbuf));
  ASSERT_EQ(0, strcmp(output, "A🍄B🌶C🍍"));

  memset(output, 0, sizeof output);

  ASSERT(ggjson_parse_node(parser, &strbuf));
  ASSERT_EQ(0, strcmp(output, "foo\nbar"));

  ASSERT(!ggjson_parse_node(parser, &strbuf));
}

int test_accept_double(ggjson_parser_state* unused, void* arg, const char* key, double value)
{
  double* output = arg;
  *output = value;
  return 1;
}

TEST(parser, double)
{
  const char* string = "123.45 -99e2";

  ggjson_string_input string_input;
  ggjson_string_input_init(&string_input, string, strlen(string));

  ggjson_parser_events events;
  ggjson_parser_events_init(&events);
  events.accept_double = test_accept_double;

  ggjson_parser* parser = &((struct parser_test_state*)data)->parser;
  ggjson_parser_init(parser, &events, (ggjson_input*)&string_input);

  double value = 0;

  ASSERT(ggjson_parse_node(parser, &value));
  ASSERT_EQ(value, 123.45);

  ASSERT(ggjson_parse_node(parser, &value));
  ASSERT_EQ(value, -99e2);

}



struct parser_test_data
{
  int true_count, false_count, null_count;
};

int test_accept_true(ggjson_parser_state* unused, void* arg, const char* key)
{
  struct parser_test_data* output = arg;
  ++output->true_count;
  return 1;
}
int test_accept_false(ggjson_parser_state* unused, void* arg, const char* key)
{
  struct parser_test_data* output = arg;
  ++output->false_count;
  return 1;
}
int test_accept_null(ggjson_parser_state* unused, void* arg, const char* key)
{
  struct parser_test_data* output = arg;
  ++output->null_count;
  return 1;
}

TEST(parser, literals)
{
  const char* string = "true false null";

  ggjson_string_input string_input;
  ggjson_string_input_init(&string_input, string, strlen(string));

  ggjson_parser_events events;
  ggjson_parser_events_init(&events);
  events.accept_true  = test_accept_true;
  events.accept_false = test_accept_false;
  events.accept_null  = test_accept_null;

  ggjson_parser* parser = &((struct parser_test_state*)data)->parser;
  ggjson_parser_init(parser, &events, (ggjson_input*)&string_input);

  struct parser_test_data ptd;
  memset(&ptd, 0, sizeof ptd);

  ASSERT(ggjson_parse_node(parser, &ptd));
  ASSERT_EQ(1, ptd.true_count);
  ASSERT_EQ(0, ptd.false_count);
  ASSERT_EQ(0, ptd.null_count);
  memset(&ptd, 0, sizeof ptd);

  ASSERT(ggjson_parse_node(parser, &ptd));
  ASSERT_EQ(0, ptd.true_count);
  ASSERT_EQ(1, ptd.false_count);
  ASSERT_EQ(0, ptd.null_count);
  memset(&ptd, 0, sizeof ptd);

  ASSERT(ggjson_parse_node(parser, &ptd));
  ASSERT_EQ(0, ptd.true_count);
  ASSERT_EQ(0, ptd.false_count);
  ASSERT_EQ(1, ptd.null_count);
  memset(&ptd, 0, sizeof ptd);

}



