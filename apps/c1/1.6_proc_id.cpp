#include "apue.h"

int main() {
  // pid를 받는 변수는 이식성을 위해 long을 사용하는 것이 좋다
  printf("hello world from process ID %ld\n", static_cast<long>(getpid()));
  return 0;
}
