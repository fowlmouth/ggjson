#include "ggjson.h"

#include <string.h>
#include <stdio.h>

int main(int argc, const char** argv)
{

  const char* json_input_str = "{\"x\":10,\"y\":100.0}";

  ggjson_string_input string_input;
  ggjson_string_input_init(&string_input, json_input_str, strlen(json_input_str));

  ggjson_input* input = (ggjson_input*)&string_input;

  for(int i = ggjson_input_read_character(input);
    i != GGJSON_EOF;
    i = ggjson_input_read_character(input))
  {
    printf("%c", (char)i);
  }
  printf("\n");

  double x = 13.37;

  return 0;  
}
