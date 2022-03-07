#ifndef GGJSON_INPUT_HEADER_INCLUDED
#define GGJSON_INPUT_HEADER_INCLUDED

#include <stddef.h>

#define GGJSON_EOF ((int)-1)

struct ggjson_input;

typedef long long ggjson_input_position_t;
typedef unsigned int ggjson_char_t;

typedef struct ggjson_input_vtable
{
  ggjson_char_t (*read_character)(struct ggjson_input*);
  void (*set_position)(struct ggjson_input*, ggjson_input_position_t);
  ggjson_input_position_t (*get_position)(struct ggjson_input*);
  int (*is_eof)(struct ggjson_input*);
} ggjson_input_vtable;

typedef struct ggjson_input
{
  struct ggjson_input_vtable* vt;
} ggjson_input;


void ggjson_input_init(struct ggjson_input* input, struct ggjson_input_vtable* vtable);

ggjson_char_t ggjson_input_read_character(struct ggjson_input* input);
void ggjson_input_set_position(struct ggjson_input* input, ggjson_input_position_t position);
ggjson_input_position_t ggjson_input_get_position(struct ggjson_input* input);
int ggjson_input_is_eof(ggjson_input*);




typedef struct ggjson_string_input
{
  struct ggjson_input base;
  const char* string;
  size_t len;
  ggjson_input_position_t pos;
} ggjson_string_input;

void ggjson_string_input_init(struct ggjson_string_input* input, const char* string, size_t string_len);



#endif
