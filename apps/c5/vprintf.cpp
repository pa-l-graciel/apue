#include "apue.h"
#include <stdarg.h>
#include <stdio.h>

void test(...) {
  va_list args;
  char* format = "%s %s\n";
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
}

int main(int args, char* argv[]) {
  test("hello", "world");
  return 0;
}
