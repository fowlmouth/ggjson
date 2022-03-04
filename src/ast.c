#include "ggjson/ast.h"

#include <stdlib.h>
#include <string.h>


typedef struct ggjson_ast_base
{
  ggjson_refcounted_object base;
  int type;
} ggjson_ast_base;




typedef struct ggjson_string
{
  ggjson_ast_base base;
  int size;
  char data[0];

} ggjson_string;




ggjson_string* ggjson_string_new(int size, const char* data)
{
  ggjson_string* str = (ggjson_string*)ggjson_alloc_refcounted(sizeof(ggjson_string) + size + 1, NULL);
  str->base.type = ggjot_string;
  str->size = size;
  if(data)
  {
    memcpy(str->data, data, size);
  }
  return str;
}




struct ggjson_ast_object_slot
{
  int hash;
  const char* key;
  ggjson_object value;
};

typedef struct ggjson_ast_object
{
  ggjson_ast_base base;

  int size, capacity_mask;
  struct ggjson_ast_object_slot* slots;

  int key_buffer_size;
  char* key_buffer_begin, *key_buffer_end;

} ggjson_ast_object;






typedef struct ggjson_ast_array
{
  ggjson_ast_base base;
  int size;
  ggjson_object* members;

} ggjson_ast_array;




enum ggjson_object_type ggjson_object_get_type(ggjson_object obj)
{
  return nan_get_type(obj);
  // int type = nanbox_get_type(obj);
  // switch(type)
  // {
  // case ggjot_real:

  //   int payload = 
  // }
  // if(nanbox_is_double(obj))
  //   return ggjot_float;
  // if(nanbox_is_int(obj))
  //   return ggjot_integer;
  // if(nanbox_is_null(obj) || nanbox_is_empty(obj))
  //   return ggjot_null;
  // if(nanbox_is_true(obj))
  //   return ggjot_true;
  // if(nanbox_is_false(obj))
  //   return ggjot_false;
  // if(nanbox_is_pointer(obj))
  // {
  //   void* value = nanbox_to_pointer(obj);

  // }

  return ggjot_error;
}

void ggjson_hold(ggjson_object obj);
void ggjson_release(ggjson_object obj);

ggjson_object ggjson_parse(ggjson_input* );


