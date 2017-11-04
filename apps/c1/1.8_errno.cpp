#include "apue.h"
#include <errno.h>

/*
 *   strerror(), perror 테스트
 */
int main(int argc, char* argv[]) {
  fprintf(stderr, "EACCESS: %s\n", strerror(EACCES));
  errno = ENOENT;
  perror(argv[0]);  // app 이름을 인자로 넘김
  exit(0);
}
