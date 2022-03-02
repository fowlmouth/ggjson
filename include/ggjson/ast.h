#ifndef GGJSON_AST_HEADER_INCLUDED
#define GGJSON_AST_HEADER_INCLUDED

#include "input.h"
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

typedef struct ggjson_ast_object_t ggjson_ast_object_t;
typedef struct ggjson_ast_array_t ggjson_ast_array_t;

enum ggjson_object_type ggjson_object_get_type(ggjson_object obj);

void ggjson_hold(ggjson_object obj);
void ggjson_release(ggjson_object obj);

ggjson_object ggjson_parse_ast(ggjson_input*);



#endif
