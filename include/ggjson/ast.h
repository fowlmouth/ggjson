#ifndef GGJSON_AST_HEADER_INCLUDED
#define GGJSON_AST_HEADER_INCLUDED

#include "input.h"
#include "util.h"
#include "refcounted.h"
#include "nanbox.h"

typedef nanbox_t ggjson_object;

enum ggjson_object_type
{
  ggjot_float = -1,
  ggjot_null, ggjot_false, ggjot_true,
  ggjot_string, ggjot_integer, 
  ggjot_object, ggjot_array,

  ggjot_error
};

typedef struct ggjson_ast_object ggjson_ast_object;
typedef struct ggjson_ast_array ggjson_ast_array;



struct ggjson_ast_object_field
{
  const char* key;
  ggjson_object value;
};

ggjson_ast_object* ggjson_ast_object_new(int size, struct ggjson_ast_object_field* members, int key_buffer_min_size);
int ggjson_ast_object_size(ggjson_ast_object*);
int ggjson_ast_object_capacity(ggjson_ast_object*);


enum ggjson_object_type ggjson_object_get_type(ggjson_object obj);

void ggjson_hold(ggjson_object obj);
void ggjson_release(ggjson_object obj);

ggjson_object ggjson_parse_ast(ggjson_input*);

#define GGJSON_INTEGER(value) \
  nan_set_int(nan_set_type(mk_nan(), ggjot_integer), (value))


#endif
