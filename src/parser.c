#include "ggjson/parser.h"

#include <string.h>

void ggjson_parser_events_init(ggjson_parser_events* parser_events)
{
  memset(parser_events, 0, sizeof(ggjson_parser_events));
}

int ggjson_parse(ggjson_parser_events* parser_events, ggjson_input* input, void* arg)
{
  return 0;
}
