#ifndef GGJSON_REFCOUNTED_HEADER_INCLUDED
#define GGJSON_REFCOUNTED_HEADER_INCLUDED

typedef struct ggjson_refcounted_object
{
  int refcount, flags;
  void(*free)(void*);
} ggjson_refcounted_object;

void* ggjson_refcounted_alloc(int size, void(*free)(void*));
void ggjson_refcounted_hold(ggjson_refcounted_object* obj);
int ggjson_refcounted_free(ggjson_refcounted_object* obj);

#endif
