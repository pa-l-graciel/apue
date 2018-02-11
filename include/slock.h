#include <semaphore.h>

typedef struct slock {
  sem_t *semp;
  char name [256];
} slock;
