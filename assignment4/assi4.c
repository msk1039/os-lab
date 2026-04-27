/*
============================================================
Producer-Consumer Using Mutex + Semaphores
------------------------------------------------------------
Idea:
- Producers place items into a bounded circular buffer.
- Consumers remove items from the same buffer.
- `empty` semaphore counts free slots; `full` counts filled slots.
- `mutex` ensures only one thread edits shared buffer indices at a time.

Detailed example:
- BUFFER_SIZE = 5, initially: empty=5, full=0, in=0, out=0.
- Producer P1 wants to insert 7:
  1) `sem_wait(empty)` makes empty 5->4 (if empty was 0, P1 would block).
  2) `pthread_mutex_lock(mutex)` enters critical section.
  3) write buffer[in=0]=7, set in=1.
  4) unlock mutex, `sem_post(full)` makes full 0->1.
- Consumer C1 wants to consume:
  1) `sem_wait(full)` makes full 1->0.
  2) lock mutex, read buffer[out=0]=7, set out=1.
  3) unlock mutex, `sem_post(empty)` makes empty 4->5.
============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define PRODUCERS   4
#define CONSUMERS   4
#define ITEMS       4

int buffer[BUFFER_SIZE];
int in = 0;   /* Next write index (circular) */
int out = 0;  /* Next read index (circular) */

/*
POSIX semaphore variables:
- sem_t empty: counts available empty slots.
- sem_t full : counts available filled slots.
*/
sem_t empty;
sem_t full;

/*
POSIX mutex variable:
- pthread_mutex_t mutex: protects shared data.
*/
pthread_mutex_t mutex;

/* Print full raw buffer array for easy visualization */
void printBuf(void) {
    printf("BUFFER: [ ");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%d ", buffer[i]);
    }
    printf("]");
}

/*
Thread entry function signature must be `void *fn(void *)` for pthread_create.
The return value is a pointer returned to pthread_join (NULL here).
*/
void *producer(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < ITEMS; i++) {
        int item = rand() % 10;

        /*
        sem_wait(&empty): decrements `empty` by 1.
        Return value: 0 on success, -1 on error.
        If `empty` is 0, this call blocks until some consumer posts `empty`.
        */
        sem_wait(&empty);

        /*
        pthread_mutex_lock(&mutex): enters critical section.
        Return value: 0 on success, non-zero error code on failure.
        */
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printBuf();
        printf(" Producer %d: put item %d at slot %d : THREAD:%lu\n",
               id, item, in, (unsigned long)pthread_self());

        /* Circular move to next insertion index */
        in = (in + 1) % BUFFER_SIZE;

        /*
        pthread_mutex_unlock(&mutex): leaves critical section.
        Return value: 0 on success, non-zero error code on failure.
        */
        pthread_mutex_unlock(&mutex);

        /*
        sem_post(&full): increments `full` by 1 and wakes one waiting consumer.
        Return value: 0 on success, -1 on error.
        */
        sem_post(&full);

        sleep(1);
    }

    return NULL;
}

void *consumer(void *arg) {
    int id = *(int *)arg;

    for (int i = 0; i < ITEMS; i++) {
        /* Wait until at least one item is available */
        sem_wait(&full);

        /* Lock before reading shared buffer/out index */
        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        printBuf();
        printf(" Consumer %d: got item %d at slot %d : THREAD:%lu\n",
               id, item, out, (unsigned long)pthread_self());

        /* Circular move to next removal index */
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);

        /* One slot became free after consume */
        sem_post(&empty);

        sleep(1);
    }

    return NULL;
}

int main(void) {
    pthread_t prod[PRODUCERS], cons[CONSUMERS];
    int pid[PRODUCERS], cid[CONSUMERS];

    /*
    sem_init(&sem, pshared, value)
    - pshared = 0 means semaphore is shared between threads of this process.
    - value is initial counter value.
    Return value: 0 on success, -1 on error.
    */
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    /*
    pthread_mutex_init(&mutex, attr)
    - attr = NULL for default mutex behavior.
    Return value: 0 on success, non-zero on error.
    */
    pthread_mutex_init(&mutex, NULL);

    /*
    pthread_create(&tid, attr, start_routine, arg)
    - starts a new thread that runs start_routine(arg).
    Return value: 0 on success, non-zero error code on failure.
    */
    for (int i = 0; i < PRODUCERS; i++) {
        pid[i] = i + 1;
        pthread_create(&prod[i], NULL, producer, &pid[i]);
    }

    for (int i = 0; i < CONSUMERS; i++) {
        cid[i] = i + 1;
        pthread_create(&cons[i], NULL, consumer, &cid[i]);
    }

    /*
    pthread_join(tid, retval)
    - waits for thread `tid` to finish.
    Return value: 0 on success, non-zero on error.
    */
    for (int i = 0; i < PRODUCERS; i++) {
        pthread_join(prod[i], NULL);
    }
    for (int i = 0; i < CONSUMERS; i++) {
        pthread_join(cons[i], NULL);
    }

    /*
    Cleanup calls:
    - sem_destroy(&sem): 0 on success, -1 on error.
    - pthread_mutex_destroy(&mutex): 0 on success, non-zero on error.
    */
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    printf("\nAll producers and consumers have finished.\n");
    return 0;
}
