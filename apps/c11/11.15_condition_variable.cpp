#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

struct msg {
  struct msg* m_next;
};

struct msg* workq;
pthread_cond_t qready;
pthread_mutex_t qlock;

void process_msg() {
  struct msg* mp;
  for (;;) {
    pthread_mutex_lock(&qlock);
    while (workq == nullptr)
      pthread_cond_wait(&qready, &qlock);
    mp = workq;
    workq = mp->m_next;
    pthread_mutex_unlock(&qlock);
    printf("msg %x is processed\n", mp);
    fflush(stdout);
    delete mp;
  }
}

void enqueue_msg(struct msg* mp) {
  pthread_mutex_lock(&qlock);
  mp->m_next = workq;
  workq = mp;
  pthread_mutex_unlock(&qlock);

  printf("enqueue msg %x\n", mp);
  fflush(stdout);
//  pthread_cond_signal(&qready);
  pthread_cond_broadcast(&qready);
//  usleep(500000); // 0.5 sec
  usleep(2000000);
//  sleep(1);   // Note: it causes hang

/*
  timespec ts;
  ts.tv_sec = 0;
  ts.tv_nsec = 500*1000*1000;
  nanosleep(&ts, nullptr);
  */
}

void sigint_handler( int signo)
{
  struct msg* mp = new msg;
  enqueue_msg(mp);
//     printf( "알람 발생 %d!!\n", counter++);
  alarm( 1);
}

void* produce(void* arg) {
  // usleep version
  for (int i = 0; i < 5; ++i) {
    struct msg* mp = new msg;
    enqueue_msg(mp);
  }

  /*
  // signal version
  signal( SIGALRM, sigint_handler);
  alarm( 1 );
  */

  pthread_exit(nullptr);
}

void* consume(void* arg) {
  process_msg();
  pthread_exit(nullptr);
}

int main() {
  setbuf(stdout, nullptr);
  pthread_t producer, consumer;
  pthread_mutex_init(&qlock, nullptr);
  pthread_cond_init(&qready, nullptr);

  pthread_create(&producer, nullptr, produce, nullptr);
  pthread_create(&consumer, nullptr, consume, nullptr);

  pthread_join(consumer, nullptr);
  pthread_join(producer, nullptr);

  return 0;
}
