#include "apue.h"
#include <pthread.h>

pthread_t ntid;

void printids(const char* s) {
  pid_t pid;
  pthread_t tid;

  pid = getpid();
  tid = pthread_self();
  // Note: linux deals thread id as unsigned long pointer
  printf("%s pid %lu tid %lu (0x%1x)\n", s, (unsigned long)pid,
         (unsigned long)tid, (unsigned long)tid);
}

void* thr_fn(void* arg) {
  printids("new thread: ");
  return ((void*)0);
}

int main() {
  int err;
  err = pthread_create(&ntid, nullptr, thr_fn, nullptr);
  if (err != 0)
    err_exit(err, "can't create thread");
  printids("main thread:");
  sleep(1);
  exit(0);
}
