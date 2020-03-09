#include "ProducerConsumer.h"

// TODO: add BoundedBuffer, locks and any global variables here

BoundedBuffer *buffer;
int produced = 0;
bool done = false;
int ps;
int cs;
auto start = std::chrono::steady_clock::now();
int item_size;
int buffer_size;
ofstream file;
pthread_mutex_t f_lock;
pthread_mutex_t p_lock;
pthread_mutex_t c_lock;

void InitProducerConsumer(int p, int c, int psleep, int csleep, int items) {
  // TODO: constructor to initialize variables declared
  // also see instruction for implementation
  buffer_size = 1;
  buffer = new BoundedBuffer(buffer_size);
  ps = psleep * 1000;
  cs = csleep * 1000;
  item_size = items;
  pthread_mutex_init(&f_lock, NULL);
  pthread_mutex_init(&c_lock, NULL);
  pthread_mutex_init(&p_lock, NULL);
  pthread_t plist[p];
  pthread_t clist[c];
  file.open("output.txt");
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
  file.close();
  pthread_mutex_destroy(&f_lock);
  pthread_mutex_destroy(&p_lock);
  pthread_mutex_destroy(&c_lock);
  delete buffer;
}

void *producer(void *threadID) {
  int pid = *(int *)threadID;
  while (1) {
    pthread_mutex_lock(&p_lock);
    if (produced == item_size) {
      done = true;
      pthread_mutex_unlock(&p_lock);
      pthread_exit(NULL);
    }
    usleep(ps);
    ++produced;
    int p_cnt = produced;
    pthread_mutex_unlock(&p_lock);
    int data = rand() % 100;
    buffer->append(data);
    auto duration = chrono::duration_cast<chrono::seconds>(
        chrono::steady_clock::now() - start);
    pthread_mutex_lock(&f_lock);
    file << "Producer #" << pid << ", ";
    file << "time = " << duration.count() << ", ";
    file << "producing data item #" << p_cnt << ", ";
    file << "item value=" << data << endl;
    pthread_mutex_unlock(&f_lock);
  }
  // TODO: producer thread, see instruction for implementation
}

void *consumer(void *threadID) {
  int pid = *(int *)threadID;
  while (1) {
    pthread_mutex_lock(&c_lock);
    if (buffer->isEmpty() && done) {
      pthread_mutex_unlock(&c_lock);
      pthread_exit(NULL);
    }
    usleep(cs);
    int data = buffer->remove();
    pthread_mutex_unlock(&c_lock);
    auto duration = chrono::duration_cast<chrono::seconds>(
        chrono::steady_clock::now() - start);
    pthread_mutex_lock(&f_lock);
    file << "Consumer #" << pid << ", ";
    file << "time = " << duration.count() << ", ";
    file << "consuming data item with value=" << data << endl;
    pthread_mutex_unlock(&f_lock);
  }
  // TODO: consumer thread, see instruction for implementation
}
