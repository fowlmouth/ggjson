#ifndef GGJSON_PARSER_HEADER_INCLUDED
#define GGJSON_PARSER_HEADER_INCLUDED

#include "input.h"

typedef struct ggjson_parser_state ggjson_parser_state;

enum ggjson_parser_actions_result
{
  ggjpar_good, ggjpar_error
};

typedef struct ggjson_parser_actions
{
  int (*begin_object)(ggjson_parser_state*, const char* key);
  int (*accept_object)(ggjson_parser_state*, const char* key);

  int (*begin_array)(ggjson_parser_state*, const char* key);
  int (*accept_array)(ggjson_parser_state*, const char* key);

  int (*accept_integer)(ggjson_parser_state*, const char* key, long long value);
  int (*accept_double)(ggjson_parser_state*, const char* key, double value);
  int (*accept_boolean)(ggjson_parser_state*, const char* key, int value);
  int (*accept_string)(ggjson_parser_state*, const char* key, int size, const char* value);
  int (*accept_null)(ggjson_parser_state*, const char* key);

  int (*on_error)(ggjson_parser_state*, const char* key, const char* message);
} ggjson_parser_actions;

int ggjson_action_parse(ggjson_parser_actions* parser_actions, ggjson_input* input, void* arg);



#endif
