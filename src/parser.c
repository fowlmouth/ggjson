#include "ggjson/parser.h"
#include "ggjson/lexer.h"
#include <string.h>

void ggjson_parser_events_init(ggjson_parser_events* parser_events)
{
  memset(parser_events, 0, sizeof(ggjson_parser_events));
}

const char* ggjson_parser_current_key(ggjson_parser* parser)
{
  // return parser->context->current_key;
  return NULL;
}

int ggjson_parser_consume_token(ggjson_parser* parser)
{
  if(!ggjson_lexer_read_token(&parser->lex, &parser->next_token, GGJSON_PARSER_ERROR_BUFFER_SIZE, parser->error_buffer))
  {
    // TODO set error code?
    return 0;
  }
  return 1;
}


int ggjson_parse_node(ggjson_parser* parser, void* arg)
{
#define CHECK_CALL(fn, ...) \
  do \
  { \
    if((fn)) (fn)(__VA_ARGS__); \
  }while(0)

  ggjson_parser_state* parser_state = (ggjson_parser_state*)parser;
  ggjson_parser_events* events = parser->events;
  switch(parser->next_token.type)
  {
  case ggjltt_integer:
    CHECK_CALL(events->accept_integer, parser_state, arg, ggjson_parser_current_key(parser), parser->next_token.int_value);
    break;

  case ggjltt_double:
    CHECK_CALL(events->accept_double, parser_state, arg, ggjson_parser_current_key(parser), parser->next_token.double_value);
    break;

  case ggjltt_string:
    CHECK_CALL(events->accept_string, parser_state, arg, ggjson_parser_current_key(parser), parser->next_token.buffer_used, parser->next_token.buffer);
    break;

  case ggjltt_true:
    CHECK_CALL(events->accept_true, parser_state, arg, ggjson_parser_current_key(parser));
    break;

  case ggjltt_false:
    CHECK_CALL(events->accept_false, parser_state, arg, ggjson_parser_current_key(parser));
    break;

  case ggjltt_null:
    CHECK_CALL(events->accept_null, parser_state, arg, ggjson_parser_current_key(parser));
    break;

  default:
    return 0;
  }

  ggjson_parser_consume_token(parser);
  return 1;
#undef CHECK_CALL
}

void ggjson_parser_init(ggjson_parser* parser, ggjson_parser_events* events, ggjson_input* input)
{
  memset(parser, 0, sizeof(ggjson_parser));
  ggjson_lexer_init(&parser->lex, input);
  parser->events = events;
  ggjson_lexer_token_init(&parser->next_token, 256);
  ggjson_parser_consume_token(parser);
}

void ggjson_parser_deinit(ggjson_parser* parser)
{
  ggjson_lexer_token_deinit(&parser->next_token);
}

int ggjson_parse(ggjson_parser_events* parser_events, ggjson_input* input, void* arg)
{
  ggjson_parser parser;
  ggjson_parser_init(&parser, parser_events, input);

  int result = ggjson_parse_node(&parser, arg);
  ggjson_parser_deinit(&parser);
  return result;
}
