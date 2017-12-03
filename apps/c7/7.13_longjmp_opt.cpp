#include "apue.h"
#include <setjmp.h>

jmp_buf jmpbuffer;
static int globval;

void f2() {
  longjmp(jmpbuffer, 1);
}

void f1(int i, int j, int k, int l) {
  printf("in f1():\n");
  printf(
      "globval = %d, autoval = %d, regival = %d, volaval = %d, statval = %d\n",
      globval, i, j, k, l);
  f2();
}

int main() {
  int autoval;
  register int regival;
  volatile int volaval;
  static int statval;

  globval = 1;
  autoval = 2;
  regival = 3;
  volaval = 4;
  statval = 5;

  /*
   * 컴파일러 최적화 시, 로컬변수는 메모리를 잡지 않고 레지스터에 곧바로
   * 저장한다
   * setjmp 호출 시, 레지스값들을 저장했다가, longjmp 호출하면 레지스터를
   * 되돌리므로일반 로컬변수들의 값은 setjmp 호출 직전의 상태로 되돌려진다.
   * 이를 방지하려면, volatile로 변수를 선언해주면 된다.
   */
  if (setjmp(jmpbuffer) != 0) {
    printf("after longjmp:\n");
    printf(
        "globval = %d, autoval = %d, regivcal = %d, volaval = %d, statval = "
        "%d\n",
        globval, autoval, regival, volaval, statval);
    exit(0);
  }
  /*
   * Change variables after setjmp, but before longjmp.
   */
  globval = 95;
  autoval = 96;
  regival = 97;
  volaval = 98;
  statval = 99;

  autoval += 1;

  f1(autoval, regival, volaval, statval); /* never returns */
  return 0;
}
