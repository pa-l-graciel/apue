#include "apue.h"
#include <fcntl.h>

char buf1[] = "abcdefghij";
char bug2[] = "ABCDEFGHIJ";

int main() {
  int fd;
  if ((fd = creat("file.hole", FILE_MODE)) < 0)
    err_sys("creat error");
  if (write(fd, buf1, 10) != 10)
    err_sys("buf1 write error");
  /* offset now = 10 */
  if (lseek(fd, 16384, SEEK_SET) == -1)
    err_sys("lseek error");
  /* offset now = 16374 */
  if (write(fd, bug2, 10) != 10)
    err_sys("buf2 write error");
  /* offset now = 16394 */

  exit(0);
}
