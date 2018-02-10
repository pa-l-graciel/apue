#include "apue.h"
#include "unistd.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main() {
  int n;
  int fd[2];
  pid_t pid;
  char line[MAXLINE];

  printf("MAXLINE: %d\n", MAXLINE);

  if (pipe(fd) < 0)
    err_sys("pipe error");
  if ((pid == fork()) < 0) {
    err_sys("fork error");
  } else if (pid > 0) {  // parent
    close(fd[0]);
    for (int i = 0; i < 10; ++i) {
      printf("write\n");
      write(fd[1], "hello world\n", 12);
      usleep(1000000);
    }
  } else {  // child
    close(fd[1]);
    int ofd;
    if ((ofd = open("out.txt", O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR)) <
        0)
      err_sys("cannot open");
    if (dup2(ofd, 1) < 0)
      err_sys("cannot dud2");

    for (int i = 0; i < 10; ++i) {
      n = read(fd[0], line, MAXLINE);

      printf("line len: %d\n", n);
      printf("%s", line);
    }
  }
  return 0;
}
