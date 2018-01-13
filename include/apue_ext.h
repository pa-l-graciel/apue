#ifndef _APUE_EXT_H
#define _APUE_EXT_H

#include <apue.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <syslog.h>

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

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)


int lockfile(int fd) {
  struct flock fl;
  fl.l_type = F_WRLCK;
  fl.l_start = 0;
  fl.l_whence = SEEK_SET;
  fl.l_len = 0;
  return fcntl(fd, F_SETLK, &fl);
}

int already_running() {
  int fd;
  char buf[16];

  fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);
  if (fd < 0) { 
    syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
    exit(1);
  }
  if (lockfile(fd) < 0) {
    if (errno == EACCES || errno == EAGAIN) {
      close(fd);
      return 1;
    }
    syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
    exit(1);
  }
  ftruncate(fd, 0);
  sprintf(buf, "%ld", (long)getpid());
  write(fd, buf, strlen(buf)+1);
  return 0;
}

#endif
