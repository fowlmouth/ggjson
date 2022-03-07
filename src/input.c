#include "ggjson/input.h"

void ggjson_input_init(struct ggjson_input* input, struct ggjson_input_vtable* vtable)
{
  input->vt = vtable;
}

ggjson_char_t ggjson_input_read_character(struct ggjson_input* input)
{
  return input->vt->read_character(input);
}

void ggjson_input_set_position(struct ggjson_input* input, ggjson_input_position_t position)
{
  return input->vt->set_position(input, position);
}

ggjson_input_position_t ggjson_input_get_position(struct ggjson_input* input)
{
  return input->vt->get_position(input);
}

int ggjson_input_is_eof(struct ggjson_input* input)
{
  return input->vt->is_eof(input);
}




int _ggjson_string_input_is_eof(struct ggjson_input* input)
{
  ggjson_string_input* string_input = (ggjson_string_input*)input;
  return string_input->pos >= string_input->len;
}

ggjson_char_t _ggjson_string_input_read_character(struct ggjson_input* input)
{
  if(_ggjson_string_input_is_eof(input))
  {
    return GGJSON_EOF;
  }
  ggjson_string_input* string_input = (ggjson_string_input*)input;
  return string_input->string[ string_input->pos++ ];
}

void _ggjson_string_input_set_position(struct ggjson_input* input, ggjson_input_position_t position)
{
  ((ggjson_string_input*)input)->pos = position;
}

ggjson_input_position_t _ggjson_string_input_get_position(struct ggjson_input* input)
{
  return ((ggjson_string_input*)input)->pos;
}

static ggjson_input_vtable string_input_vt = {
  .read_character = _ggjson_string_input_read_character,
  .set_position = _ggjson_string_input_set_position,
  .get_position = _ggjson_string_input_get_position,
  .is_eof = _ggjson_string_input_is_eof
};

void ggjson_string_input_init(struct ggjson_string_input* input, const char* string, size_t string_len)
{
  ggjson_input_init(&input->base, &string_input_vt);
  input->string = string;
  input->len = string_len;
  input->pos = 0;
}

