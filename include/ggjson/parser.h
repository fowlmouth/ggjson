#ifndef GGJSON_PARSER_HEADER_INCLUDED
#define GGJSON_PARSER_HEADER_INCLUDED

#include "lexer.h"

typedef struct ggjson_parser_state ggjson_parser_state;

enum ggjson_parser_events_result
{
  ggjper_good, ggjper_error
};

typedef struct ggjson_parser_events
{
  int (*begin_object)(ggjson_parser_state*, void*, const char* key);
  int (*accept_object)(ggjson_parser_state*, void*, const char* key);

  int (*begin_array)(ggjson_parser_state*, void*, const char* key);
  int (*accept_array)(ggjson_parser_state*, void*, const char* key);

  int (*accept_integer)(ggjson_parser_state*, void*, const char* key, long long value);
  int (*accept_double)(ggjson_parser_state*, void*, const char* key, double value);
  int (*accept_boolean)(ggjson_parser_state*, void*, const char* key, int value);
  int (*accept_string)(ggjson_parser_state*, void*, const char* key, int size, const char* value);
  int (*accept_true)(ggjson_parser_state*, void*, const char* key);
  int (*accept_false)(ggjson_parser_state*, void*, const char* key);
  int (*accept_null)(ggjson_parser_state*, void*, const char* key);

  int (*on_error)(ggjson_parser_state*, void*, const char* key, const char* message);
} ggjson_parser_events;


void ggjson_parser_events_init(ggjson_parser_events* parser_events);

int ggjson_parse(ggjson_parser_events* parser_events, ggjson_input* input, void* arg);



#ifndef GGJSON_PARSER_ERROR_BUFFER_SIZE
# define GGJSON_PARSER_ERROR_BUFFER_SIZE 512
#endif

typedef struct ggjson_parser
{
  ggjson_parser_events* events;
  ggjson_lexer lex;
  ggjson_lexer_token next_token;
  char error_buffer[GGJSON_PARSER_ERROR_BUFFER_SIZE];
} ggjson_parser;

void ggjson_parser_init(ggjson_parser* parser, ggjson_parser_events* events, ggjson_input* input);
void ggjson_parser_deinit(ggjson_parser* parser);
int ggjson_parse_node(ggjson_parser* parser, void* arg);

#endif
