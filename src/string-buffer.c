#include "string-buffer.h"

struct ggjson_string_buffer* ggjson_string_buffer_new(struct ggjson_string_buffer* next, int size)
{
  struct ggjson_string_buffer* buf = calloc(sizeof(struct ggjson_string_buffer), 1);
  buf->next = next;
  buf->size = size;
  buf->buffer_begin = buf->buffer_end = calloc(1, size);
  return buf;
}

void ggjson_string_buffer_free(struct ggjson_string_buffer* buf)
{
  if(!buf)
  {
    return;
  }
  struct ggjson_string_buffer* next = buf->next;
  buf->next = NULL;
  ggjson_string_buffer_free(next);
  free(buf->buffer_begin);
  free(buf);
}

int ggjson_string_buffer_space_remaining(struct ggjson_string_buffer* buf)
{
  return buf->size - (buf->buffer_end - buf->buffer_begin);
}

int ggjson_string_buffer_write(struct ggjson_string_buffer* buf, int size, const char* string, char** output)
{
  int space_remaining = ggjson_string_buffer_space_remaining(buf);
  int to_write = space_remaining < size ? space_remaining : size;
  memcpy(buf->buffer_end, string, to_write);
  if(output)
  {
    *output = buf->buffer_end;
  }
  buf->buffer_end += to_write;
  return to_write;
}
