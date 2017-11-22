#include "apue.h"

#define BSZ 48

int main() {
  FILE *fp;
  char buf[BSZ];

  memset(buf, 'a', BSZ-1);
  buf[BSZ-2] = '\0';
  buf[BSZ-1] = 'X';
  if ((fp = fmemopen(buf, BSZ, "w+")) == nullptr)
    err_sys("fmemopen failed");
  printf("initial buffer contents: %s\n", buf);
  fprintf(fp, "hello, world");  // 12 characters
  printf("before flush: %s\n", buf);
  fflush(fp);
  printf("after fflush: %s\n", buf);
  printf("len of string in buf = %ld\n", (long)strlen(buf));  // result: 12

  memset(buf, 'b', BSZ-2);
  buf[BSZ-2] = '\0';
  buf[BSZ-1] = 'X';
  fprintf(fp, "hello, world");
  fseek(fp, 0, SEEK_SET);   // fseek causes flush
  printf("after fseek: %s\n", buf);
  printf("len of string in buf = %ld\n", (long)strlen(buf)); // result: 24

  memset(buf, 'c', BSZ-2);
  buf[BSZ-2] = '\0';
  buf[BSZ-1] = 'X';
  fprintf(fp, "hello, world");
  fclose(fp);
  printf("after fclose: %s\n", buf);
  printf("len of string in buf = %ld\n", (long)strlen(buf)); // result: 46

  
  return 0;
}
