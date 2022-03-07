#include "libtest.h"

#include "ggjson/lexer.h"

#include <string.h>

TEST(lexer, tokens) {
  const char* string = "[ 10, 20, 30 ]";
  const int token_count = 7;

  const int error_buffer_size = 256;
  char error_buffer[error_buffer_size];

  ggjson_string_input string_input;
  ggjson_string_input_init(&string_input, string, strlen(string));
 
  ggjson_lexer lexer;
  ggjson_lexer_init(&lexer, (ggjson_input*)&string_input);

  ggjson_lexer_token token __attribute__((cleanup(ggjson_lexer_token_deinit)));
  ggjson_lexer_token_init(&token, 256);

  for(int i = 0; i < token_count; ++i)
  {
    ASSERT_MSGF(
      ggjson_lexer_read_token(&lexer, &token, error_buffer_size, error_buffer),
      "expected a token %d/%d", i, token_count);
    printf("token type= '%d' pos= '%lld' \n", token.type, token.begin.position);
  }

  ASSERT_MSG(
    ggjson_lexer_read_token(&lexer, &token, error_buffer_size, error_buffer),
    "expected a token (EOF)");
  ASSERT_EQ(token.type, ggjltt_eof);

  PASS();
}


