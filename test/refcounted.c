#include "libtest.h"

#include "ggjson/refcounted.h"

int test_free_refcounted_counter;

void test_free_refcounted(void* obj)
{
  (void)obj;
  --test_free_refcounted_counter;
}

TEST(refcounted, free)
{
  void* obj = ggjson_refcounted_alloc(32, test_free_refcounted);
  test_free_refcounted_counter = 1;

  ASSERT_LT(ggjson_refcounted_free(obj), 1);
  ASSERT_EQ(0, test_free_refcounted_counter);

  PASS();
}
