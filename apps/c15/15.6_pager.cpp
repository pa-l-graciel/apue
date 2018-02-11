#include "apue.h"
#include <sys/wait.h>

#define DEF_PAGER "/bin/more"

/*
/bin/more는stdin을받으면내용을stdout으로출력한다.
부모는 a.out의 내용을 읽어서 파이프에 쓰고,
자식은 파이프를 읽는다. 이 때 자식은 미리 dup2를 호출하여,
읽기파이프는 STDIN_FILENO 교체되어 있다.
따라서 fd[1]에 write하면 자식프로세스의stdin으로 들어간다.
*/

int main(int argc, char* argv[]) {
  int n;
  int fd[2];
  pid_t pid;
  char *pager, *argv0;
  char line[MAXLINE];
  FILE* fp;

  if (argc != 2)
    err_quit("usage: a.out <pathname>");
  if ((fp = fopen(argv[1], "r")) == nullptr)
    err_sys("can't open %s", argv[1]);
  if (pipe(fd) < 0)
    err_sys("pipe error");
  if ((pid = fork()) < 0) {
    err_sys("fork error");
  } else if (pid > 0) {
    close(fd[0]);
    while (fgets(line, MAXLINE, fp) != nullptr) {
      n = strlen(line);
      if (write(fd[1], line, n) != n)
        err_sys("write error to pipe");
    }
    if (ferror(fp))
      err_sys("fgets error");
    close(fd[1]);
    if (waitpid(pid, nullptr, 0) < 0)
      err_sys("waitpid error");
    exit(0);
  } else {
    close(fd[1]);
    if (fd[0] != STDIN_FILENO) {
      if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
        err_sys("dup2 error to stdin");
      close(fd[0]);
    }

    if ((pager = getenv("PAGER")) == nullptr)
      pager = DEF_PAGER;
    if ((argv0 = strrchr(pager, '/')) != nullptr)
      argv0++;
    else
      argv0 = pager;

    if (execl(pager, argv0, (char*)0) < 0)
      err_sys("ececl error for %s", pager);
  }
  exit(0);
}
