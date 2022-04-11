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



#define PARSER_TEST_DATA_BUFFER_SIZE 64

struct parser_test_data
{
  int int_count, true_count, false_count, null_count;
  long long integer_value;
  double double_value;
  char string_buffer[PARSER_TEST_DATA_BUFFER_SIZE];
};

int test_accept_integer(ggjson_parser_state* unused, void* arg, const char* key, long long value)
{
  // long long* output = arg;
  struct parser_test_data* output = arg;
  ++output->int_count;
  output->integer_value = value;
  return 1;
}

int test_accept_string(ggjson_parser_state* unused, void* arg, const char* key, int size, const char* str)
{
  struct parser_test_data* output = arg;
  snprintf(output->string_buffer, PARSER_TEST_DATA_BUFFER_SIZE, "%s", str);
  return 1;
}

int test_accept_double(ggjson_parser_state* unused, void* arg, const char* key, double value)
{
  struct parser_test_data* output = arg;
  output->double_value = value;
  return 1;
}

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


void init_test_events(ggjson_parser_events* ev)
{
  ggjson_parser_events_init(ev);
  ev->accept_integer = test_accept_integer;
  ev->accept_string  = test_accept_string;
  ev->accept_double  = test_accept_double;
  ev->accept_true    = test_accept_true;
  ev->accept_false   = test_accept_false;
  ev->accept_null    = test_accept_null;
}



TEST(parser, integer)
{
  const char* string = "12345 -99 42";

  ggjson_string_input string_input;
  ggjson_string_input_init(&string_input, string, strlen(string));

  ggjson_parser_events events;
  init_test_events(&events);

  struct parser_test_state* test_state = data;
  ggjson_parser* parser = &test_state->parser;
  ggjson_parser_init(parser, &events, (ggjson_input*)&string_input);

  struct parser_test_data ptd;
  memset(&ptd, 0, sizeof ptd);

  ASSERT(ggjson_parse_node(parser, &ptd));
  ASSERT_EQ(ptd.int_count, 1);
  ASSERT_EQ(ptd.integer_value, 12345);
  memset(&ptd, 0, sizeof ptd);

  ASSERT(ggjson_parse_node(parser, &ptd));
  ASSERT_EQ(ptd.int_count, 1);
  ASSERT_EQ(ptd.integer_value, -99);
  memset(&ptd, 0, sizeof ptd);

  ASSERT(ggjson_parse_node(parser, &ptd));
  ASSERT_EQ(ptd.int_count, 1);
  ASSERT_EQ(ptd.integer_value, 42);
  memset(&ptd, 0, sizeof ptd);

  ASSERT(ggjson_input_is_eof((ggjson_input*)&string_input));
  ASSERT(!ggjson_parse_node(parser, &ptd));
}

TEST(parser, string)
{
  const char* string = "\"A🍄B🌶C🍍\" \"foo\nbar\"";

  ggjson_string_input string_input;
  ggjson_string_input_init(&string_input, string, strlen(string));

  ggjson_parser_events events;
  init_test_events(&events);

  struct parser_test_state* test_state = data;
  ggjson_parser* parser = &test_state->parser;
  ggjson_parser_init(parser, &events, (ggjson_input*)&string_input);

  struct parser_test_data ptd;
  memset(&ptd, 0, sizeof ptd);

  ASSERT(ggjson_parse_node(parser, &ptd));
  ASSERT_EQ(0, strcmp(ptd.string_buffer, "A🍄B🌶C🍍"));
  memset(&ptd, 0, sizeof ptd);

  ASSERT(ggjson_parse_node(parser, &ptd));
  ASSERT_EQ(0, strcmp(ptd.string_buffer, "foo\nbar"));
  memset(&ptd, 0, sizeof ptd);

  ASSERT(!ggjson_parse_node(parser, &ptd));
}

TEST(parser, double)
{
  const char* string = "123.45 -99e2";

  ggjson_string_input string_input;
  ggjson_string_input_init(&string_input, string, strlen(string));

  ggjson_parser_events events;
  init_test_events(&events);

  struct parser_test_state* test_state = data;
  ggjson_parser* parser = &test_state->parser;
  ggjson_parser_init(parser, &events, (ggjson_input*)&string_input);

  struct parser_test_data ptd;
  memset(&ptd, 0, sizeof ptd);

  ASSERT(ggjson_parse_node(parser, &ptd));
  ASSERT_EQ(ptd.double_value, 123.45);
  memset(&ptd, 0, sizeof ptd);

  ASSERT(ggjson_parse_node(parser, &ptd));
  ASSERT_EQ(ptd.double_value, -99e2);
  memset(&ptd, 0, sizeof ptd);

  ASSERT(!ggjson_parse_node(parser, &ptd));
}


TEST(parser, literals)
{
  const char* string = "true false null";

  ggjson_string_input string_input;
  ggjson_string_input_init(&string_input, string, strlen(string));

  ggjson_parser_events events;
  init_test_events(&events);

  struct parser_test_state* test_state = data;
  ggjson_parser* parser = &test_state->parser;
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

  ASSERT(!ggjson_parse_node(parser, &ptd));
}



