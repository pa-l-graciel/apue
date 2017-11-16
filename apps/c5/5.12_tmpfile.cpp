#include "apue.h"

int main() {
  char name[L_tmpnam], line[MAXLINE];
  FILE* fp;
  printf("%s\n", tmpnam(nullptr));

  tmpnam(name);  // warning: unsafe function
  printf("%s\n", name);

  char name2[L_tmpnam];
  snprintf(name2, L_tmpnam, "uniqueXXXXXX");  
  int fd = mkstemp(name2);  // will not automatically removed
  printf("%s\n", name2);

  if ((fp = tmpfile()) == nullptr)
    err_sys("tmpfile error");
  fputs("one line of output\n", fp);
  rewind(fp);
  if (fgets(line, sizeof(line), fp) == nullptr)
    err_sys("fgets error");
  fputs(line, stdout);
  exit(0);
}
