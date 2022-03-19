#include "ggjson.h"
#include "libtest.h"
#include <locale.h>

int main(int argc, char** argv)
{
  setlocale(LC_ALL, "");
  RUN_TESTS();

  return 0;
}
