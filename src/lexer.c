#include "ggjson/lexer.h"

#include <stdlib.h>
#include <string.h>

void ggjson_lexer_init(ggjson_lexer* lexer, ggjson_input* input)
{
  
}

void ggjson_lexer_token_init(ggjson_lexer_token* token, int buffer_capacity)
{
  memset(token, 0, sizeof(struct ggjson_lexer_token));
  token->buffer_capacity = buffer_capacity;
  token->buffer = calloc(1, buffer_capacity);
}

void ggjson_lexer_token_deinit(ggjson_lexer_token* token)
{
  token->buffer = realloc(token->buffer, 0);
}

int ggjson_lexer_read_token(ggjson_lexer* lexer, ggjson_lexer_token* token, int error_buffer_size, char* error_buffer)
{
  ggjson_lexer_skip_whitespace(lexer);

  token->type = ggjltt_eof;
  token->begin = token->end = ggjson_lexer_get_position(lexer);

  return 0;
}