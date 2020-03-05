#include "BoundedBuffer.h"

BoundedBuffer::BoundedBuffer(int N) {
    // TODO: constructor to initiliaze all the varibales declared in
    // BoundedBuffer.h
    buffer_size = N;
    buffer_cnt = 0;
    buffer_first = 0;
    buffer_last = 0;
    int buffer_array[buffer_size];
    buffer = buffer_array;
    pthread_mutex_init(&buffer_lock, NULL);
    pthread_cond_init(&buffer_not_full, NULL);
    pthread_cond_init(&buffer_not_empty, NULL);
}

BoundedBuffer::~BoundedBuffer() {
    // TODO: destructor to clean up anything necessary
    pthread_mutex_destroy(&buffer_lock);
    pthread_cond_destroy(&buffer_not_empty);
    pthread_cond_destroy(&buffer_not_full);
}

void BoundedBuffer::append(int data) {
    // TODO: append a data item to the circular buffer
    pthread_mutex_lock(&buffer_lock);
    while (buffer_cnt==buffer_size)
    {
        pthread_cond_wait(&buffer_not_full, &buffer_lock);
    }
    buffer[buffer_last] = data;
    buffer_last = (buffer_last + 1) % buffer_size;
    ++buffer_cnt;
    pthread_cond_signal(&buffer_not_empty);
    pthread_mutex_unlock(&buffer_lock);
}

int BoundedBuffer::remove() {
    // TODO: remove and return a data item from the circular buffer
    pthread_mutex_lock(&buffer_lock);
    int data;
    while (isEmpty())
    {
        pthread_cond_wait(&buffer_not_empty, NULL);
    }
    data = buffer[buffer_first];
    buffer_first = (buffer_first + 1) % buffer_size;
    pthread_cond_signal(&buffer_not_full);
    pthread_mutex_unlock(&buffer_lock);
    return data;
}

bool BoundedBuffer::isEmpty() {
    // TODO: check is the buffer is empty
    return buffer_size == 0;
}
