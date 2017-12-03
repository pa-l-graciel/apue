#include "apue.h"
#include <pthread.h>

void cleanup(void* arg) {
  printf("cleanup: %s\n", (char*)arg);
}

void* thr_fn1(void* arg) {
  printf("thread 1 start\n");
  pthread_cleanup_push(
      cleanup, static_cast<void*>(const_cast<char*>("thread 1 first handler")));
  pthread_cleanup_push(
      cleanup,
      static_cast<void*>(const_cast<char*>("thread 1 second handler")));
  printf("thread 1 push complete\n");
  if (arg) {
    printf("thread1 has arg\n");

    // FIXME: pthread_clean_push와 pthread_cleanup_pop 사이에  return으로
    // 끝내면 BSD에서 세그폴트가 나는 이식성 이슈가 발생한다. 따라서 return으로
    // 끝내지 말고 pthread_exit으로 종료하도록 해야한다.
    return ((void*)1);
  }
  pthread_cleanup_pop(0);
  pthread_cleanup_pop(0);

  return ((void*)1);
}

void* thr_fn2(void* arg) {
  printf("thread 2 start\n");
  pthread_cleanup_push(
      cleanup, static_cast<void*>(const_cast<char*>("thread 2 first handler")));
  pthread_cleanup_push(
      cleanup,
      static_cast<void*>(const_cast<char*>("thread 2 second handler")));
  printf("thread 2 push complete\n");
  if (arg)
    pthread_exit((void*)2);
  pthread_cleanup_pop(0);
  pthread_cleanup_pop(0);
  pthread_exit((void*)2);
}

int main() {
  int err;
  pthread_t tid1, tid2;
  void* tret;
  //  err = pthread_create(&tid1, NULL, thr_fn1, (void*)1);
  err = pthread_create(&tid1, NULL, thr_fn1, nullptr);
  if (err != 0)
    err_exit(err, "can’t create thread 1");
  err = pthread_create(&tid2, NULL, thr_fn2, (void*)1);
  if (err != 0)
    err_exit(err, "can’t create thread 2");
  err = pthread_join(tid1, &tret);
  if (err != 0)
    err_exit(err, "can’t join with thread 1");
  printf("thread 1 exit code %ld\n", (long)tret);
  err = pthread_join(tid2, &tret);
  if (err != 0)
    err_exit(err, "can’t join with thread 2");
  printf("thread 2 exit code %ld\n", (long)tret);
  exit(0);
}
