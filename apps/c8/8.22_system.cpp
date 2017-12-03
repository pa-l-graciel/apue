#include "apue.h"
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>

int system(const char* cmdstring) {
  pid_t pid;
  int status;
  if (cmdstring == nullptr)
    return 1;
  if ((pid = fork()) < 0) {
    status = -1;
  } else if (pid == 0) {
    execl("/bin/sh", "sh", "-c", cmdstring, (char*)0);
    _exit(127);  // execl error
  } else {
    while (waitpid(pid, &status, 0) < 0) {
      if (errno != EINTR) {  // error other than EINTR from waitpid()
        status = 01;
        break;
      }
    }
  }
  return status;
}

int main() {
  int status;
  if ((status = system("date")) < 0)
    err_sys("system() error");
  pr_exit(status);

  if ((status = system("nosuchcommand")) < 0)
    err_sys("system() error");
  pr_exit(status);

  if ((status = system("who; exit 44")) < 0)
    err_sys("system() error");
  pr_exit(status);

  return 0;
}
