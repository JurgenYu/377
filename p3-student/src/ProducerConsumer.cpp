#include "ProducerConsumer.h"

// TODO: add BoundedBuffer, locks and any global variables here

BoundedBuffer buffer;
int p_cnt = 0;
int c_cnt = 0;
int ps;
int cs;
auto start = std::chrono::steady_clock::now();
int item_size;
pthread_mutex_t p_lock;
pthread_mutex_t c_lock;

void InitProducerConsumer(int p, int c, int psleep, int csleep, int items) {
  // TODO: constructor to initialize variables declared
  // also see instruction for implementation
  BoundedBuffer buffer(10);
  ps = psleep;
  cs = csleep;
  item_size = items;
  pthread_mutex_init(&p_lock, NULL);
  pthread_mutex_init(&c_lock, NULL);
  pthread_t plist[p];
  pthread_t clist[c];
  for (int i = 0; i < p; i++) {
    int out = pthread_create(&plist[i], NULL, producer, &i);
    if (out) {
      perror("Producer Initiation Error");
    }
  }
  for (int j = 0; j < c; j++) {
    int out = pthread_create(&clist[j], NULL, consumer, &j);
    if (out) {
      perror("Consumer Initiation Error");
    }
  }
  for (int a = 0; a < p; a++) {
    int out = pthread_join(plist[a], NULL);
    if (out) {
      perror("Producer Join Error");
    }
  }
  for (int b = 0; b < c; b++) {
    int out = pthread_join(clist[b], NULL);
    if (out) {
      perror("Consumer Join Error");
    }
  }
  pthread_mutex_destroy(&p_lock);
  pthread_mutex_destroy(&c_lock);
}

void *producer(void *threadID) {
  int produced = 0;
  while (1) {
    usleep(ps);
    pthread_mutex_lock(&p_lock);
    if (p_cnt == item_size)
      ;
    { break; }
    produced+=1;
    int data = rand() % 100;
    buffer.append(data);
    ++p_cnt;
    cout << "Producer #" << threadID << ", ";
    time = current time, producing data item #j,
                         item value = foo pthread_mutex_unlock(&p_lock);
  }
  // TODO: producer thread, see instruction for implementation
}

void *consumer(void *threadID) {
  // TODO: consumer thread, see instruction for implementation
}
