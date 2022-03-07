#include "libtest.h"

#include "ggjson/lexer.h"

#include <string.h>



struct lexer_test_state
{
  ggjson_lexer_token token;
};

void lexer_test_setup(void* arg)
{
  struct lexer_test_state* state = arg;
  ggjson_lexer_token_init(&state->token, 256);
}

void lexer_test_destroy(void* arg)
{
  struct lexer_test_state* state = arg;
  ggjson_lexer_token_deinit(&state->token);
}

SUITE(lexer, sizeof(struct lexer_test_state), lexer_test_setup, lexer_test_destroy)




TEST(lexer, tokens)
{
  const char* string = "[ 10, 20, 30 ]";
  const int token_count = 7;

  const int error_buffer_size = 256;
  char error_buffer[error_buffer_size];

  struct lexer_test_state* state = data;
  ggjson_string_input string_input;
  ggjson_string_input_init(&string_input, string, strlen(string));
 
  ggjson_lexer lexer;
  ggjson_lexer_init(&lexer, (ggjson_input*)&string_input);

  ggjson_lexer_token *token = &state->token;

  for(int i = 0; i < token_count; ++i)
  {
    ASSERT_MSGF(
      ggjson_lexer_read_token(&lexer, token, error_buffer_size, error_buffer),
      "expected a token %d/%d", i, token_count);
  }

  ASSERT_MSG(
    ggjson_lexer_read_token(&lexer, token, error_buffer_size, error_buffer),
    "expected a token (EOF)");
  ASSERT_EQ(token->type, ggjltt_eof);

  PASS();
}

TEST(lexer, integers)
{
  const char* string = "98765 -43210";

  const int error_buffer_size = 256;
  char error_buffer[error_buffer_size];

  struct lexer_test_state* state = data;
  ggjson_string_input string_input;
  ggjson_string_input_init(&string_input, string, strlen(string));
 
  ggjson_lexer lexer;
  ggjson_lexer_init(&lexer, (ggjson_input*)&string_input);

  ggjson_lexer_token *token = &state->token;

  ASSERT(ggjson_lexer_read_token(&lexer, token, error_buffer_size, error_buffer));
  ASSERT_EQ(token->type, ggjltt_integer);
  ASSERT_EQ(token->int_value, 98765);

  ASSERT(ggjson_lexer_read_token(&lexer, token, error_buffer_size, error_buffer));
  ASSERT_EQ(token->type, ggjltt_integer);
  ASSERT_EQ(token->int_value, -43210);

  ASSERT_EQ(ggjlrtr_eof, ggjson_lexer_read_token(&lexer, token, error_buffer_size, error_buffer));
  ASSERT_EQ(token->type, ggjltt_eof);

  PASS();
}

TEST(lexer, punctuation)
{
  const char* string = "[]{},:";
  const int token_count = 6;

  const int error_buffer_size = 256;
  char error_buffer[error_buffer_size];

  struct lexer_test_state* state = data;
  ggjson_string_input string_input;
  ggjson_string_input_init(&string_input, string, strlen(string));
 
  ggjson_lexer lexer;
  ggjson_lexer_init(&lexer, (ggjson_input*)&string_input);

  ggjson_lexer_token *token = &state->token;

  for(int i = 0; i < token_count; ++i)
  {
    ASSERT(ggjson_lexer_read_token(&lexer, token, error_buffer_size, error_buffer));
  }

  ASSERT(ggjson_lexer_read_token(&lexer, token, error_buffer_size, error_buffer));
  ASSERT_EQ(token->type, ggjltt_eof);

  PASS();

}
