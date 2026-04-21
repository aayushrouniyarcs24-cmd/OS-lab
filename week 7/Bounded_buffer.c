#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define MAX_ITEMS 15

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;


sem_t empty;
sem_t full;
sem_t mutex;


int produce_item(int i) {
    return i;
}


void consume_item(int item, int index) {
    printf("Consumed: %d from buffer[%d]\n", item, index);
}


void* producer(void* arg) {
    int next_produced;

    for (int i = 0; i < MAX_ITEMS; i++) {

        next_produced = produce_item(i);

        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[in] = next_produced;
        printf("Produced: %d at buffer[%d]\n", next_produced, in);
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&full);

        usleep(100000);
    }
    return NULL;
}


void* consumer(void* arg) {
    int next_consumed;

    for (int i = 0; i < MAX_ITEMS; i++) {
        sem_wait(&full);
        sem_wait(&mutex);

        next_consumed = buffer[out];
        int index = out;
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&empty);

        consume_item(next_consumed, index);

        usleep(150000);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;


    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);


    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);


    pthread_join(prod, NULL);
    pthread_join(cons, NULL);


    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
