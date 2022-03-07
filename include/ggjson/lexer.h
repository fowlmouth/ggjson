#ifndef GGJSON_LEXER_HEADER_INCLUDED
#define GGJSON_LEXER_HEADER_INCLUDED

#include "input.h"

typedef enum ggjson_lexer_token_type
{
  ggjltt_eof,
  ggjltt_colon, ggjltt_comma,
  ggjltt_open_brace, ggjltt_close_brace,
  ggjltt_open_bracket, ggjltt_close_bracket,
  ggjltt_null, ggjltt_true, ggjltt_false,
  ggjltt_integer, ggjltt_real, ggjltt_string
} ggjson_lexer_token_type;

typedef struct ggjson_lexer_token_position
{
  ggjson_input_position_t position;
  long line, col;
} ggjson_lexer_token_position;

typedef struct ggjson_lexer_token
{
  ggjson_lexer_token_type type;
  ggjson_lexer_token_position begin, end;
  long long int_value;
  int buffer_capacity;
  char* buffer;
} ggjson_lexer_token;

typedef enum ggjson_lexer_read_token_result
{
  ggjlrtr_error, ggjlrtr_eof, ggjlrtr_token
} ggjson_lexer_read_token_result;

typedef struct ggjson_lexer
{
  ggjson_input* input;
  long line, col;
  ggjson_char_t current_character;
} ggjson_lexer;

void ggjson_lexer_init(ggjson_lexer*, ggjson_input*);
void ggjson_lexer_token_init(ggjson_lexer_token*, int);
void ggjson_lexer_token_deinit(ggjson_lexer_token*);

int ggjson_lexer_read_token(ggjson_lexer*, ggjson_lexer_token*, int, char*);


#endif
