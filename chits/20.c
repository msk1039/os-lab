

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 5
#define ITEMS 10

int buffer[SIZE], in = 0, out = 0;

sem_t empty, full;

pthread_mutex_t mutex;

void *producer(void *arg) {
    for (int i = 1; i <= ITEMS; i++) {

        sem_wait(&empty);


        pthread_mutex_lock(&mutex);


        buffer[in] = i;
        printf("Produced %d at index %d\n", i, in);


        in = (in + 1) % SIZE;


        pthread_mutex_unlock(&mutex);


        sem_post(&full);
        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 1; i <= ITEMS; i++) {

        sem_wait(&full);
        pthread_mutex_lock(&mutex);


        int item = buffer[out];
        printf("Consumed %d from index %d\n", item, out);
        out = (out + 1) % SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(1);
    }
    return NULL;
}

int main(void) {
    pthread_t p, c;


    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);


    pthread_mutex_init(&mutex, NULL);


    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);


    pthread_join(p, NULL);
    pthread_join(c, NULL);


    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
