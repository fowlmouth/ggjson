#include "ggjson/refcounted.h"

#include <stdlib.h>
#include <assert.h>

void* ggjson_alloc_refcounted(int size, void(*free)(void*))
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

void ggjson_hold_refcounted(ggjson_refcounted_object* obj)
{
  ++obj->refcount;
}

void ggjson_free_refcounted(ggjson_refcounted_object* obj)
{
  if(! --obj->refcount)
  {
    if(obj->free)
      obj->free(obj);
    free(obj);
  }
}

