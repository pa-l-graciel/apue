/* 
 * 아래와 같은 함수를 사용하면, sysconf가 -1을 리턴해서 무한루프에 빠진다.
 * #include <unistd.h>
 * for (i = 0; i < sysconf(_SC_OPEN_MAX); i++)
 *   close(i);
 */

#include "apue.h"
#include <errno.h>
#include <limits.h>

#ifdef OPEN_MAX
static long openmax = OPEN_MAX;
#else
static long openmax = 0;
#endif

/*
 * OPEN_MAX가 불확정인 경우를 위찬 추정치. 적합하지 않을 수 있다.
 */
#define OPEN_MAX_GUESS 256

long open_max() {
  if (openmax == 0) { // 첫 시도
    printf("OPEN_MAX is not defined. Trying to get value from sysconf...\n");
    errno = 0;
    if ((openmax = sysconf(_SC_OPEN_MAX)) < 0) {
      if (errno == 0)   // 불확정인 경우
        openmax = OPEN_MAX_GUESS;
      else
        err_sys("sysconf error for _SCOPEN_MAX");
    }
  }
  return openmax;
}

int main() {
  printf("Result of open_max(): %ld\n", open_max());
  return 0;
}
