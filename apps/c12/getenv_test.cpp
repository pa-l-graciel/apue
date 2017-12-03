#include "apue.h"
#include "apue_ext.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRINGSZ 4096

char envbuf[MAXSTRINGSZ];
extern char** environ;

// MT-unsafe version
char* getenv(const char* name) {
  int i, len;
  len = strlen(name);
  for (i = 0; environ[i] != nullptr; ++i) {
    if ((strncmp(name, environ[i], len) == 0) && (environ[i][len] == '=')) {
      strncpy(envbuf, &environ[i][len + 1], MAXSTRINGSZ - 1);
      return envbuf;
    }
  }
  return nullptr;
}

pthread_mutex_t env_mutex;
pthread_once_t init_done = PTHREAD_ONCE_INIT;

void thread_init() {
  pthread_mutexattr_t attr;

  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&env_mutex, &attr);
  pthread_mutexattr_destroy(&attr);
}

// MT-safe: get buffer from argument
int getenv_r(const char* name, char* buf, int buflen) {
  int i, len, olen;

  pthread_once(&init_done, thread_init);
  len = strlen(name);
  pthread_mutex_lock(&env_mutex);
  for (i = 0; environ[i] != nullptr; ++i) {
    if ((strncmp(name, environ[i], len) == 0) && (environ[i][len] == '=')) {
      olen = strlen(&environ[i][len + 1]);
      if (olen >= buflen) {
        pthread_mutex_unlock(&env_mutex);
        return (ENOSPC);
      }
      strcpy(buf, &environ[i][len + 1]);
      pthread_mutex_unlock(&env_mutex);
      return 0;
    }
  }
  pthread_mutex_unlock(&env_mutex);
  return ENOENT;
}

void* mt_safe_getenv(void* arg) {
  char buf[MAXSTRINGSZ];
  getenv_r((const char*)arg, buf, sizeof(buf));
  printf("%s\n", buf);
  return 0;
}

// MT-safe: using key
pthread_key_t key;
pthread_once_t init_done_key = PTHREAD_ONCE_INIT;
pthread_mutex_t env_mutex_key = PTHREAD_MUTEX_INITIALIZER;

void thread_init_key() {
  pthread_key_create(&key, free);
}

char* getenv_k(const char* name) {
  int i, len;
  char* envbuf;
  pthread_once(&init_done_key, thread_init_key);
  pthread_mutex_lock(&env_mutex_key);
  envbuf = (char*)pthread_getspecific(key);

  if (envbuf == nullptr) {
    envbuf = (char*)malloc(MAXSTRINGSZ);
    if (envbuf == nullptr) {
      pthread_mutex_unlock(&env_mutex_key);
      return nullptr;
    }
    pthread_setspecific(key, envbuf);
  }
  len = strlen(name);
  for (i = 0; environ[i] != nullptr; ++i) {
    if ((strncmp(name, environ[i], len) == 0) && (environ[i][len] == '=')) {
      strncpy(envbuf, &environ[i][len + 1], MAXSTRINGSZ - 1);
      pthread_mutex_unlock(&env_mutex_key);
      return envbuf;
    }
  }
  pthread_mutex_unlock(&env_mutex_key);
  return nullptr;
}

void* mt_safe_getenv_key(void* arg) {
  printf("%s\n", getenv_k((const char*)arg));
  return 0;
}

int main() {
  setbuf(stdout, nullptr);
  pthread_t tid0, tid1;

  // Test 0: Using no thread-safe version of get-env
  printf("###TEST MT-UNSAFE GETENV\n");
  tid0 = makethread((void* (*)(void*))getenv, (void*)"PATH");
  tid1 = makethread((void* (*)(void*))getenv, (void*)"HOME");
  printf("%s\n", envbuf);
  pthread_join(tid0, nullptr);
  pthread_join(tid1, nullptr);

  // Test 1: Using thread-safe version of get-env
  printf("\n###TEST MT-SAFE GETENV\n");
  tid0 = makethread((void* (*)(void*))mt_safe_getenv, (void*)"PATH");
  tid1 = makethread((void* (*)(void*))mt_safe_getenv, (void*)"HOME");
  pthread_join(tid0, nullptr);
  pthread_join(tid1, nullptr);

  // Test 1: Using thread-safe version of get-env
  printf("\n###TEST MT-SAFE GETENV USING KEY\n");
  tid0 = makethread((void* (*)(void*))mt_safe_getenv_key, (void*)"PATH");
  tid1 = makethread((void* (*)(void*))mt_safe_getenv_key, (void*)"HOME");
  pthread_join(tid0, nullptr);
  pthread_join(tid1, nullptr);

  tid0 = makethread((void* (*)(void*))mt_safe_getenv_key, (void*)"PATH");
  tid1 = makethread((void* (*)(void*))mt_safe_getenv_key, (void*)"HOME");
  pthread_join(tid0, nullptr);
  pthread_join(tid1, nullptr);

  exit(0);
}
