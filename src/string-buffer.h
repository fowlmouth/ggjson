#ifndef GGJSON_STRING_BUFFER_HEADER_INCLUDED

#include <stdlib.h>
#include <string.h>

typedef struct ggjson_string_buffer
{
  struct ggjson_string_buffer* next;
  int size;
  char* buffer_begin, *buffer_end;
} ggjson_string_buffer;

struct ggjson_string_buffer* ggjson_string_buffer_new(struct ggjson_string_buffer* next, int size);
void ggjson_string_buffer_free(struct ggjson_string_buffer* buf);
int ggjson_string_buffer_space_remaining(struct ggjson_string_buffer* buf);
int ggjson_string_buffer_write(struct ggjson_string_buffer* buf, int size, const char* string, char** output);


#endif
