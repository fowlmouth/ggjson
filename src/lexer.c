#include "ggjson/lexer.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

ggjson_char_t ggjson_lexer_next_character(ggjson_lexer* lexer);

void ggjson_lexer_init(ggjson_lexer* lexer, ggjson_input* input)
{
  memset(lexer, 0, sizeof(struct ggjson_lexer));
  lexer->input = input;
  lexer->line = 1;
  lexer->col = -1;
  ggjson_lexer_next_character(lexer);  
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

ggjson_lexer_token_position ggjson_lexer_get_position(ggjson_lexer* lexer)
{
  ggjson_lexer_token_position pos = {
    .position = ggjson_input_get_position(lexer->input),
    .line = lexer->line,
    .col = lexer->col
  };
  return pos;
}

ggjson_char_t ggjson_lexer_next_character(ggjson_lexer* lexer)
{
  ggjson_char_t c = ggjson_input_read_character(lexer->input);
  if(c == '\n')
  {
    ++lexer->line;
    lexer->col = 0;
  }
  else
  {
    ++lexer->col;
  }
  return lexer->current_character = c;
}

ggjson_char_t ggjson_lexer_current_character(ggjson_lexer* lexer)
{
  return lexer->current_character;
}

int ggjson_lexer_skip_whitespace(ggjson_lexer* lexer)
{
  int count = 0;
  while(lexer->current_character == ' ')
  {
    ggjson_lexer_next_character(lexer);
    ++count;
  }
  return count;
}


int ggjson_lexer_read_token(ggjson_lexer* lexer, ggjson_lexer_token* token, int error_buffer_size, char* error_buffer)
{
  ggjson_lexer_skip_whitespace(lexer);

  token->type = ggjltt_eof;
  token->begin = token->end = ggjson_lexer_get_position(lexer);
  char* str = token->buffer, *str_end = str + token->buffer_capacity;
  *str = 0;

#define TOKEN_WRITE(character) \
  *str++ = character;

  switch(lexer->current_character)
  {
  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7': case '8': case '9':
    token->type = ggjltt_integer;
    token->int_value = 0;
    while(isdigit(ggjson_lexer_current_character(lexer)))
    {
      token->int_value = (token->int_value * 10) + (ggjson_lexer_current_character(lexer) - '0');
      *str++ = (char)ggjson_lexer_current_character(lexer);

    }
    // TODO check for ., e, etc here for floats
    *str++ = 0;
  }

  return 0;
}