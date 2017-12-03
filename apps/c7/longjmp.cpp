#include "apue.h"
#include <setjmp.h>

jmp_buf jmpbuf;

void func1(int c) {
  printf("func1 c = %d\n", c);
  if (c == 1) {
    longjmp(jmpbuf, 1);
  }
}

void func2(int c) {
  printf("func2 c = %d\n", c);
  if (c == 3) {
    longjmp(jmpbuf, 2);
  }
}

int main() {
  int e;
  int count = 0;

  if ((e = setjmp(jmpbuf)) != 0)
    printf("error occured in function %d\n", e);

  func1(++count);
  func2(++count);

  return 0;
}
