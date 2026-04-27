/*
============================================================
Producer-Consumer Using POSIX Threads
------------------------------------------------------------
Idea:
- Producers and consumers run as separate threads.
- Shared circular buffer holds produced items.
- Semaphores handle availability (`empty`, `full`), mutex guards shared state.

Detailed example:
- Suppose BUFFER_SIZE=5, count=0, in=0, out=0.
- Producer creates item 101:
  1) `sem_wait(empty)` -> empty decrements.
  2) lock mutex, insert at `in`, increment `in`, increment `count`.
  3) unlock mutex, `sem_post(full)` -> full increments.
- Consumer removes next item:
  1) `sem_wait(full)` -> waits if no data.
  2) lock mutex, remove at `out`, increment `out`, decrement `count`.
  3) unlock mutex, `sem_post(empty)` -> signals one free slot.
============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

/* Configuration */
#define BUFFER_SIZE        5
#define NUM_PRODUCERS      3
#define NUM_CONSUMERS      3
#define ITEMS_PER_PRODUCER 6

typedef struct {
    int data[BUFFER_SIZE];
    int in;     /* Next insertion index */
    int out;    /* Next removal index */
    int count;  /* Number of valid items currently present */
} SharedBuffer;

static SharedBuffer buf = { .in = 0, .out = 0, .count = 0 };

/*
Synchronization objects:
- pthread_mutex_t mutex:
  protects shared structure `buf` during insert/remove operations.
- sem_t empty:
  counts empty buffer slots (initially BUFFER_SIZE).
- sem_t full:
  counts filled buffer slots (initially 0).
*/
static pthread_mutex_t mutex;
static sem_t empty;
static sem_t full;

static void timestamp(char *buf_ts, size_t len) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm *tm_info = localtime(&ts.tv_sec);
    snprintf(buf_ts, len, "%02d:%02d:%02d.%03ld",
             tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec,
             ts.tv_nsec / 1000000L);
}

/* Called only while mutex is locked */
static void insert_item(int item) {
    buf.data[buf.in] = item;
    buf.in = (buf.in + 1) % BUFFER_SIZE;
    buf.count++;
}

/* Called only while mutex is locked */
static int remove_item(void) {
    int item = buf.data[buf.out];
    buf.out = (buf.out + 1) % BUFFER_SIZE;
    buf.count--;
    return item;
}

static void print_buffer(void) {
    printf("    Buffer [");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        int idx = (buf.out + i) % BUFFER_SIZE;
        if (i < buf.count) {
            printf("%3d", buf.data[idx]);
        } else {
            printf("  _");
        }
    }
    printf(" ] (count=%d)\n", buf.count);
}

typedef struct { int id; } ProducerArg;

/*
pthread thread start routine rules:
- Signature must be `void *(*)(void *)`.
- Return pointer is delivered to pthread_join (unused -> NULL).
*/
static void *producer(void *arg) {
    ProducerArg *parg = (ProducerArg *)arg;
    char ts[32];

    for (int i = 0; i < ITEMS_PER_PRODUCER; i++) {
        usleep((rand() % 300 + 100) * 1000);
        int item = parg->id * 100 + i;

        /*
        sem_wait(&empty):
        - blocks if empty==0
        - returns 0 on success, -1 on error
        */
        sem_wait(&empty);

        /*
        pthread_mutex_lock(&mutex):
        - enters critical section
        - returns 0 on success, non-zero error code on failure
        */
        pthread_mutex_lock(&mutex);
        timestamp(ts, sizeof ts);
        insert_item(item);
        printf("[%s] PRODUCER-%d produced item %3d -> ", ts, parg->id, item);
        print_buffer();

        /*
        pthread_mutex_unlock(&mutex):
        - leaves critical section
        - returns 0 on success, non-zero error code on failure
        */
        pthread_mutex_unlock(&mutex);

        /*
        sem_post(&full):
        - increments full counter
        - wakes one blocked consumer if any
        - returns 0 on success, -1 on error
        */
        sem_post(&full);
    }

    timestamp(ts, sizeof ts);
    printf("[%s] PRODUCER-%d finished.\n", ts, parg->id);
    return NULL;
}

typedef struct { int id; int target; int total_consumed; } ConsumerArg;

static void *consumer(void *arg) {
    ConsumerArg *carg = (ConsumerArg *)arg;
    char ts[32];

    while (carg->total_consumed < carg->target) {
        sem_wait(&full);

        pthread_mutex_lock(&mutex);
        int item = remove_item();
        carg->total_consumed++;
        timestamp(ts, sizeof ts);
        printf("[%s] CONSUMER-%d consumed item %3d <- ", ts, carg->id, item);
        print_buffer();
        pthread_mutex_unlock(&mutex);

        sem_post(&empty);
        usleep((rand() % 400 + 150) * 1000);
    }

    timestamp(ts, sizeof ts);
    printf("[%s] CONSUMER-%d finished (consumed %d items).\n",
           ts, carg->id, carg->total_consumed);
    return NULL;
}

int main(void) {
    srand((unsigned)time(NULL));

    int total_items = NUM_PRODUCERS * ITEMS_PER_PRODUCER;

    printf("=== Producer-Consumer Problem ===\n");
    printf("Buffer size : %d\n", BUFFER_SIZE);
    printf("Producers   : %d (each produces %d items)\n", NUM_PRODUCERS, ITEMS_PER_PRODUCER);
    printf("Consumers   : %d\n", NUM_CONSUMERS);
    printf("Total items : %d\n\n", total_items);

    /*
    pthread_mutex_init(&mutex, NULL)
    - creates default mutex
    - returns 0 on success, non-zero on error
    */
    pthread_mutex_init(&mutex, NULL);

    /*
    sem_init(&sem, pshared, initial_value)
    - pshared=0 => shared between threads in same process
    - returns 0 on success, -1 on error
    */
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    pthread_t prod_threads[NUM_PRODUCERS];
    ProducerArg prod_args[NUM_PRODUCERS];

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        prod_args[i].id = i + 1;

        /*
        pthread_create(...)
        - starts new thread running producer(&prod_args[i])
        - returns 0 on success, non-zero error code on failure
        */
        if (pthread_create(&prod_threads[i], NULL, producer, &prod_args[i]) != 0) {
            perror("pthread_create producer");
            return EXIT_FAILURE;
        }
    }

    pthread_t cons_threads[NUM_CONSUMERS];
    ConsumerArg cons_args[NUM_CONSUMERS];

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        cons_args[i].id = i + 1;
        cons_args[i].total_consumed = 0;

        /* Distribute total items among consumers as evenly as possible */
        cons_args[i].target = total_items / NUM_CONSUMERS;
        if (i < total_items % NUM_CONSUMERS) {
            cons_args[i].target++;
        }

        if (pthread_create(&cons_threads[i], NULL, consumer, &cons_args[i]) != 0) {
            perror("pthread_create consumer");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        /*
        pthread_join(thread, NULL)
        - waits until thread exits
        - returns 0 on success, non-zero on error
        */
        pthread_join(prod_threads[i], NULL);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(cons_threads[i], NULL);
    }

    /*
    Resource cleanup:
    - pthread_mutex_destroy: 0 on success, non-zero on error
    - sem_destroy          : 0 on success, -1 on error
    */
    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    printf("\n=== All threads finished. Program complete. ===\n");
    return EXIT_SUCCESS;
}
