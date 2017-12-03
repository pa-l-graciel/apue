#include "apue.h"

int globvar = 6; /* initialized global variable */
char buf[] = "a write to stdout\n";

int main() {
  int var;
  pid_t pid;

  var = 88;
  if (write(STDOUT_FILENO, buf, sizeof(buf) - 1) != sizeof(buf) - 1)
    err_sys("write error");
  printf("before fork\n");  // we don't flush stdout

  if ((pid = vfork()) < 0) {
    err_sys("fork error");
  } else if (pid == 0) {  // child
    globvar++;
    var++;
    _exit(0);
  }

  // parent continues here
  printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);
  return 0;
}
