#include "ProducerConsumer.h"

// TODO: add BoundedBuffer, locks and any global variables here

BoundedBuffer *buffer;
int p_cnt = 0;
int c_cnt = 0;
int ps;
int cs;
auto start = std::chrono::steady_clock::now();
int item_size;
pthread_mutex_t lock;

void InitProducerConsumer(int p, int c, int psleep, int csleep, int items) {
  // TODO: constructor to initialize variables declared
  // also see instruction for implementation
  buffer = new BoundedBuffer(10);
  ps = psleep;
  cs = csleep;
  item_size = items;
  pthread_mutex_init(&lock, NULL);
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
  pthread_mutex_destroy(&lock);
  delete buffer;
}

void *producer(void *threadID) {
  int produced = 0;
  while (1) {
    usleep(ps);
    pthread_mutex_lock(&lock);
    if (p_cnt == item_size) {
      break;
    }
    produced += 1;
    int data = rand() % 100;
    buffer->append(data);
    ++p_cnt;
    auto duration = chrono::duration_cast<chrono::seconds>(
        chrono::steady_clock::now() - start);
    fstream file;
    file.open("output.txt");
    file << "Producer #" << threadID << ", ";
    file << "time = " << duration.count() << ", ";
    file << "producing data item #" << produced << ", ";
    file << "item value=" << data << endl;
    pthread_mutex_unlock(&lock);
  }
  return 0;
  // TODO: producer thread, see instruction for implementation
}

void *consumer(void *threadID) {
  int consumed = 0;
  while (1) {
    usleep(cs);
    pthread_mutex_lock(&lock);
    if (c_cnt == item_size) {
      break;
    }
    consumed += 1;
    int data = buffer->remove();
    ++c_cnt;
    auto duration = chrono::duration_cast<chrono::seconds>(
        chrono::steady_clock::now() - start);
    fstream file;
    file.open("output.txt");
    file << "Consumer #" << threadID << ", ";
    file << "time = " << duration.count() << ", ";
    file << "consuming data item with value=" << data << endl;
    pthread_mutex_unlock(&lock);
  }
  return 0;
  // TODO: consumer thread, see instruction for implementation
}
