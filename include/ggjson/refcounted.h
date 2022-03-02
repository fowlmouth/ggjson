#ifndef GGJSON_REFCOUNTED_HEADER_INCLUDED
#define GGJSON_REFCOUNTED_HEADER_INCLUDED

typedef struct ggjson_refcounted_object
{
  int refcount, flags;
  void(*free)(void*);
} ggjson_refcounted_object;

void* ggjson_alloc_refcounted(int size, void(*free)(void*));
void ggjson_hold_refcounted(ggjson_refcounted_object* obj);
void ggjson_free_refcounted(ggjson_refcounted_object* obj);

#endif
