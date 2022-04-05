#include "ggjson/ast.h"
#include "string-buffer.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define GGJSON_AST_OBJECT_KEYBUFFER_MIN_SIZE 256

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
  ggjson_string* str = (ggjson_string*)ggjson_refcounted_alloc(sizeof(ggjson_string) + size + 1, NULL);
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
  unsigned int hash;
  const char* key;
  ggjson_object value;
};


struct ggjson_ast_object
{
  ggjson_ast_base base;

  int size, capacity_mask;
  struct ggjson_ast_object_slot* slots;

  struct ggjson_string_buffer* key_buffer;

};


void ggjson_ast_object_free(void* ptr)
{
  ggjson_ast_object* obj = ptr;
  free(obj->slots);
  ggjson_string_buffer_free(obj->key_buffer);
}


unsigned int djb2(const char* str)
{
  unsigned int h = 5381;
  for(char c = *str++; c; c = *str++)
    h = (h << 5) + h + c;
  return h;
}


void ggjson_ast_object_grow(ggjson_ast_object* obj)
{
  int old_capacity = obj->capacity_mask + 1,
    new_capacity = old_capacity * 2,
    new_capacity_mask = new_capacity - 1;
  struct ggjson_ast_object_slot* old_slots = obj->slots,
    *new_slots = calloc(sizeof(struct ggjson_ast_object_slot), new_capacity);
  int size = obj->size;
  for(int i = 0; i < old_capacity && size; ++i)
  {
    struct ggjson_ast_object_slot* old_slot = old_slots + i;
    if(old_slot->key)
    {
      --size;
      int index = old_slot->hash & new_capacity_mask;
      while(new_slots[index].key)
      {
        index = (index + 1) & new_capacity_mask;
      }
      struct ggjson_ast_object_slot* new_slot = new_slots + index;
      new_slot->hash  = old_slot->hash;
      new_slot->key   = old_slot->key;
      new_slot->value = old_slot->value;
    }
  }
  assert(size == 0);
  obj->capacity_mask = new_capacity_mask;
  obj->slots = new_slots;
  free(old_slots);
}

void ggjson_ast_object_grow_key_buffer(ggjson_ast_object* obj, int min_length)
{
  if(ggjson_string_buffer_space_remaining(obj->key_buffer) < min_length)
  {
    obj->key_buffer = ggjson_string_buffer_new(obj->key_buffer, min_length ? next_power_of_two(min_length) : min_length);
  }
}

char* ggjson_ast_object_key(ggjson_ast_object* obj, const char* str)
{
  int len = strlen(str);
  if(! obj->key_buffer)
  {
    obj->key_buffer = ggjson_string_buffer_new(NULL, len < 1024 ? next_power_of_two(len+1) : (len+1));
  }
  ggjson_ast_object_grow_key_buffer(obj, len+1);
  char* result;
  int written = ggjson_string_buffer_write(obj->key_buffer, len+1, str, &result);
  assert(written == len+1);
  return result;
}

void ggjson_ast_object_insert(ggjson_ast_object* obj, const char* key, ggjson_object value)
{
  const unsigned int hash = djb2(key);
  int index;
  struct ggjson_ast_object_slot* slot;
lookup:
  index = (int)(hash & obj->capacity_mask);
  slot = obj->slots + index;
  while(slot->key)
  {
    if(slot->hash == hash && !strcmp(slot->key, key))
    {
      // insert here
      slot->value = value;
      return;
    }
    index = (index + 1) & obj->capacity_mask;
    slot = obj->slots + index;
  }
  // slot is at an empty entry, see if we must grow to accomodate
  if(obj->size == obj->capacity_mask)
  {
    ggjson_ast_object_grow(obj);
    goto lookup;
  }
  slot->hash  = hash;
  slot->key   = ggjson_ast_object_key(obj, key);
  slot->value = value;
}

ggjson_ast_object* ggjson_ast_object_new(int size, struct ggjson_ast_object_field* members, int key_buffer_min_size)
{
  // int capacity = next_power
  int capacity = size ? next_power_of_two(size + 1) : 4;
  ggjson_ast_object* obj = (ggjson_ast_object*)ggjson_refcounted_alloc(sizeof(ggjson_ast_object), ggjson_ast_object_free);
  obj->base.type = ggjot_string;
  obj->size = size;
  obj->capacity_mask = capacity - 1;
  int key_buffer_size,
    actual_member_count = 0;
  if(size && members)
  {
    int strbuf_size = 0;
    for(int i = 0; i < size; ++i)
    {
      struct ggjson_ast_object_field* field = members + i;
      if(!field->key)
      {
        break;
      }
      ++actual_member_count;
      strbuf_size += strlen(field->key) + 1;
    }
    key_buffer_size = next_power_of_two(strbuf_size > key_buffer_min_size ? strbuf_size : key_buffer_min_size);
  }
  else
  {
    key_buffer_size = key_buffer_min_size ? next_power_of_two(key_buffer_min_size) : GGJSON_AST_OBJECT_KEYBUFFER_MIN_SIZE;
  }
  ggjson_string_buffer* key_buffer = ggjson_string_buffer_new(NULL, key_buffer_size);

  // TODO handle this better
  if(!key_buffer)
  {
    goto cleanup;
  }
  obj->key_buffer = key_buffer;

  struct ggjson_ast_object_slot* slots = calloc(sizeof(struct ggjson_ast_object_slot), capacity);
  if(!slots)
  {
    goto cleanup;
  }
  obj->slots = slots;

  for(int i = 0; i < actual_member_count; ++i)
  {
    ggjson_ast_object_insert(obj, members[i].key, members[i].value);
  }

  return obj;

cleanup:
  // ggjson_string_buffer_free(key_buffer); // not necessary: this is stored inside obj and free'd in the next line
  ggjson_ast_object_free(obj);
  return NULL;
}

int ggjson_ast_object_size(ggjson_ast_object* obj)
{
  return obj ? obj->size : -1;
}
int ggjson_ast_object_capacity(ggjson_ast_object* obj)
{
  return obj ? obj->capacity_mask + 1 : -1;
}





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


