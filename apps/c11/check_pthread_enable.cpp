#include <stdio.h>
#include <pthread.h>

#ifdef _POSIX_THREADS
const int pthread_available = true;
#else
const int pthread_available = false;
#endif

int main() {
  printf("pthread available: %d\n", pthread_available);
  return 0;
}
