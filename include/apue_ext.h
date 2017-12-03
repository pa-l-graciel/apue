#ifndef _APUE_EXT_H
#define _APUE_EXT_H

#include <apue.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

pthread_t makethread(void* (*fn)(void*),
                     void* arg,
                     int detachstate = PTHREAD_CREATE_JOINABLE) {
  int err;
  pthread_t tid;
  pthread_attr_t attr;

  err = pthread_attr_init(&attr);
  if (err != 0)
    return err;
  //  err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  err = pthread_attr_setdetachstate(&attr, detachstate);
  if (err == 0) {
    err = pthread_create(&tid, &attr, fn, arg);
  }
  pthread_attr_destroy(&attr);

  return tid;
}
#endif
