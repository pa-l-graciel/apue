#include "apue.h"

int main() {
  char* ptr;
  size_t size;
  if (chdir("/var/lock") < 0) /* warning: chdir follows symlink */
    err_sys("chdir failed");

  ptr = path_alloc(&size); /* our own function */
  if (getcwd(ptr, size) == nullptr)
    err_sys("getcwd failed");
  printf("cwd = %s\n", ptr);

  exit(0);
}
