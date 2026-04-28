/*
Problem Statement (Chit 23):
Implement the C program for bounded buffer producer- consumer problem.
*/

/*
============================================================
Bounded Buffer Producer-Consumer Problem
------------------------------------------------------------
Idea:
- Producer thread inserts items into a fixed-size buffer.
- Consumer thread removes items from the same buffer.
- Semaphore empty counts empty slots.
- Semaphore full counts filled slots.
- Mutex protects shared variables in, out, and buffer.

Detailed example:
- Buffer size = 5, initially empty=5 and full=0.
- Producer calls sem_wait(empty), locks mutex, inserts item, unlocks mutex,
  then calls sem_post(full).
- Consumer calls sem_wait(full), locks mutex, removes item, unlocks mutex,
  then calls sem_post(empty).
============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 5
#define ITEMS 10

int buffer[SIZE], in = 0, out = 0;
/* `empty` counts available free slots; `full` counts produced items. */
sem_t empty, full;
/* Mutex serializes updates to shared buffer indices and elements. */
pthread_mutex_t mutex;

void *producer(void *arg) {
    for (int i = 1; i <= ITEMS; i++) {
        /*
        sem_wait(&empty):
        - decreases empty slot count
        - blocks if count is 0
        - returns 0 on success, -1 on error
        */
        sem_wait(&empty);

        /*
        pthread_mutex_lock(&mutex):
        - enters critical section
        - returns 0 on success, non-zero error code on failure
        */
        pthread_mutex_lock(&mutex);

        /* Place next produced item at current write position. */
        buffer[in] = i;
        printf("Produced %d at index %d\n", i, in);

        /* Move write index in circular manner. */
        in = (in + 1) % SIZE;

        /*
        pthread_mutex_unlock(&mutex):
        - leaves critical section
        - returns 0 on success, non-zero error code on failure
        */
        pthread_mutex_unlock(&mutex);

        /*
        sem_post(&full):
        - increases filled slot count
        - wakes waiting consumer if any
        - returns 0 on success, -1 on error
        */
        sem_post(&full);
        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 1; i <= ITEMS; i++) {
        /*
        Wait for at least one produced item before trying to consume.
        This prevents reading from an empty buffer in normal semaphore flow.
        */
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        /* Read item from current consume position and move index forward. */
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

    /*
    sem_init(&sem, pshared, value):
    - pshared=0 means shared between threads of same process
    - value is initial count
    - returns 0 on success, -1 on error
    */
    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);

    /*
    pthread_mutex_init(&mutex, NULL):
    - initializes default mutex
    - returns 0 on success, non-zero on error
    */
    pthread_mutex_init(&mutex, NULL);

    /*
    pthread_create(&thread, attr, function, arg):
    - starts a new thread
    - returns 0 on success, non-zero on error
    */
    /* Start one producer and one consumer thread. */
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    /*
    pthread_join(thread, NULL):
    - waits for thread to finish
    - returns 0 on success, non-zero on error
    */
    pthread_join(p, NULL);
    pthread_join(c, NULL);

    /*
    Cleanup:
    - sem_destroy returns 0 on success, -1 on error
    - pthread_mutex_destroy returns 0 on success, non-zero on error
    */
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
