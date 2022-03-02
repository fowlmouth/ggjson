#ifndef GGJSON_HEADER_INCLUDED
#define GGJSON_HEADER_INCLUDED

#include "ggjson/input.h"
#include <stddef.h>

struct ggjson_parser_context_t
{
  int line, col;
  void* userdata;
};

enum ggjson_parser_action_result
{
  ggjpar_continue_parsing,
  ggjpar_error
};

struct ggjson_parser_actions_t
{
  enum ggjson_parser_action_result (*accept_string)(struct ggjson_parser_context_t*, const char* key, const char* string, size_t string_size);
  enum ggjson_parser_action_result (*accept_integer)(struct ggjson_parser_context_t*, const char* key, long long number);
  enum ggjson_parser_action_result (*accept_float)(struct ggjson_parser_context_t*, const char* key, double number);

  enum ggjson_parser_action_result (*accept_true)(struct ggjson_parser_context_t*, const char* key);
  enum ggjson_parser_action_result (*accept_false)(struct ggjson_parser_context_t*, const char* key);
  enum ggjson_parser_action_result (*accept_null)(struct ggjson_parser_context_t*, const char* key);

  enum ggjson_parser_action_result (*begin_object)(struct ggjson_parser_context_t*, const char* key);
  enum ggjson_parser_action_result (*accept_object)(struct ggjson_parser_context_t*, const char* key);
  
  enum ggjson_parser_action_result (*begin_array)(struct ggjson_parser_context_t*, const char* key);
  enum ggjson_parser_action_result (*accept_array)(struct ggjson_parser_context_t*, const char* key);
};


// void ggjson_parser_



#endif