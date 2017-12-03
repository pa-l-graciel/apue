#include "apue.h"
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

int makethread(void* (*fn)(void*), void* arg) {
  int err;
  pthread_t tid;
  pthread_attr_t attr;

  err = pthread_attr_init(&attr);
  if (err != 0)
    return err;
  err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  if (err == 0) {
    err = pthread_create(&tid, &attr, fn, arg);
  }
  pthread_attr_destroy(&attr);

  pthread_join(tid, nullptr);
  return err;
}

struct to_info {
  void (*to_fn)(void*);     // function
  void* to_arg;             // argument
  struct timespec to_wait;  // time to wait
};

#define SECTONSEC 1000000000  // nanosecond

#if !defined(CLOCK_REALTIME) || defined(BSD)
#define cloack_nanosleep(ID, FL, REQ, REM) nanosleep((REQ), (REM))
#endif

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#define USECTONSEC 1000  // microseconds to nanoseconds

void clock_gettime(int id, struct timespec* tsp) {
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  tsp->tv_sec = tv.tv_sec;
  tsp->tv_nsec = tv.tv_usec * USECTONSEC;
}
#endif

void* timeout_helper(void* arg) {
  struct to_info* tip;
  tip = (struct to_info*)arg;
  clock_nanosleep(CLOCK_REALTIME, 0, &tip->to_wait, nullptr);
  (*tip->to_fn)(tip->to_arg);
  free(arg);
  return 0;
}

void timeout(const struct timespec* when, void (*func)(void*), void* arg) {
  struct timespec now;
  struct to_info* tip;
  int err;

  /*
  clock_gettime(CLOCK_REALTIME, &now);
  if ((when->tv_sec > now.tv_sec) ||
      (when->tv_sec == now.tv_sec && when->tv_nsec > now.tv_nsec)) {
    tip = (struct to_info*)malloc(sizeof(struct to_info));
    if (tip != nullptr) {
      tip->to_fn = func;
      tip->to_arg = arg;
      tip->to_wait.tv_sec = when->tv_sec - now.tv_sec;
      if (when->tv_nsec >= now.tv_nsec) {
        tip->to_wait.tv_nsec = when->tv_nsec - now.tv_nsec;
      } else {
        tip->to_wait.tv_sec--;
        tip->to_wait.tv_nsec = SECTONSEC - now.tv_nsec + when->tv_nsec;
      }
      printf("set time\n");
      err = makethread(timeout_helper, (void*)tip);
      if (err == 0)
        return;
      else
        free(tip);
    }
  }
  */
  // we get here if (a) when <= now, or (b) mallock fails, or
  // (c) we can't make a thread, so we just call the function now
  printf("timeout retry...\n");
  (*func)(arg);
}

pthread_mutexattr_t attr;
pthread_mutex_t mutex;

void retry(void* arg) {
  printf("0x%lx: try lock(%lx)\n", pthread_self(), &mutex);
  pthread_mutex_lock(&mutex);
  printf("0x%lx: lock(%lx) complete\n", pthread_self(), &mutex);
  /* perform retry steps... */
  pthread_mutex_unlock(&mutex);
  printf("0x%lx: unlock(%lx)\n", pthread_self(), &mutex);
}

int main() {
  int err, condition, arg;
  struct timespec when;

  condition = 1;

  if ((err = pthread_mutexattr_init(&attr)) != 0)
    err_exit(err, "pthread_mutexattr_init failed");
  /*
  if ((err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE)) != 0)
  */
  if ((err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL)) != 0)
    err_exit(err, "can't set recursive type");
  if ((err = pthread_mutex_init(&mutex, &attr)) != 0)
    err_exit(err, "can't create recursive mutex");

  /* continue processing ... */

  pthread_mutex_lock(&mutex);
  printf("0x%lx: lock(0x%lx)\n", pthread_self(), &mutex);

  // Check the condition under the protection of a lock to
  // make the check and the call to timeout atomic.

  if (condition) {
    // Calculate the absolute time when we want to retry.
    clock_gettime(CLOCK_REALTIME, &when);
    when.tv_sec += 2;  // sec from now
    timeout(&when, retry, (void*)((unsigned long)arg));
  }
  pthread_mutex_unlock(&mutex);
  printf("0x%lx: unlock(0x%lx)\n", pthread_self(), &mutex);

  /* continue processing ... */

  exit(0);
}
