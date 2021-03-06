#include "ggjson/refcounted.h"

#include <stdlib.h>
#include <assert.h>

void* ggjson_refcounted_alloc(int size, void(*free)(void*))
{
  assert(size >= sizeof(struct ggjson_refcounted_object));
  ggjson_refcounted_object* obj = (ggjson_refcounted_object*)calloc(size, 1);
  if(!obj)
  {
    return NULL;
  }
  obj->refcount = 0;
  obj->flags = 0;
  obj->free = free;
  return obj;
}

void ggjson_refcounted_hold(ggjson_refcounted_object* obj)
{
  ++obj->refcount;
}

int ggjson_refcounted_free(ggjson_refcounted_object* obj)
{
  int result = --obj->refcount;
  if(result < 1)
  {
    if(obj->free)
      obj->free(obj);
    free(obj);
  }
  return result;
}

