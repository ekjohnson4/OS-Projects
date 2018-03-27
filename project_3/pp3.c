#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define BUFFER_SIZE 100

typedef int buffer_item;
typedef sem_t Sema;
Sema *empty, *full;
pthread_mutex_t mutex;

buffer_item buffer[BUFFER_SIZE];
int frontIndex = 0, backIndex = 0, true = 1;

Sema *makeSemaphore(int count) {
    Sema *s = malloc(sizeof(Sema));
    if (sem_init(s, 0, count) != 0) {
        fprintf(stderr, "Error creating semaphore\n");
        exit(1);
    }
    return s;
}

int insertItem(buffer_item item) {
    int semFlag = sem_wait(empty);

    if (semFlag != 0){
      return -1;
    }

    pthread_mutex_lock(&mutex);
    buffer[backIndex] = item;
    backIndex++;

    if (backIndex == BUFFER_SIZE) {
        backIndex = 0;
    }
    pthread_mutex_unlock(&mutex);

    return sem_post(full);
}

int removeItem(buffer_item *item) {
    int semFlag = sem_wait(full);

    if (semFlag != 0){
      return -1;
    }

    pthread_mutex_lock(&mutex);
    *item = buffer[frontIndex];
    frontIndex++;

    if (frontIndex == BUFFER_SIZE) {
        frontIndex = 0;
    }
    pthread_mutex_unlock(&mutex);

    return sem_post(empty);
}

void *produce(void *arg) {
    int *threadNum = arg;
    printf("Producer thread %d created.\n", *threadNum);
    buffer_item item;

    while (true) {
        sleep(rand() % 4);
        item = rand();

        if (insertItem(item)) {
            fprintf(stderr, "Error with producing.\n");
        }
        else {
            printf("Thread %d producing %d\n", *threadNum, item);
        }
    }
    return NULL;
}

void *consume(void *arg) {
    int *threadNum = arg;
    printf("Consumer thread %d created.\n", *threadNum);
    buffer_item item;

    while (true) {
        sleep(rand() % 4);
        if (removeItem(&item)) {
            fprintf(stderr, "Error with consumption\n");
        }
        else {
            printf("Thread %d consuming %d\n", *threadNum, item);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    // 1. Get command line arguments argv[1],argv[2],argv[3]
    if (argc < 4) {
        fprintf(stderr, "Please provide arguments.\n");
        exit(1);
    }

    int timeSleep = atoi(argv[1]);
    int producerThreads = atoi(argv[2]);
    int consumerThreads = atoi(argv[3]);

    pthread_t proudcers[producerThreads];
    pthread_t consumers[consumerThreads];

    //2. Initialize buffer
    empty = makeSemaphore(BUFFER_SIZE);
    full = makeSemaphore(0);

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Failed to init mutex.\n");
        return 1;
    }

    //3. Create producer thread(s)
    for (int i = 0; i < producerThreads; i++) {
        int *index = malloc(sizeof(int));
        *index = i + 1;
        pthread_create(&proudcers[i], NULL, produce, index);
    }

    //4. Create consumer thread(s)
    for (int i = 0; i < consumerThreads; i++) {
        int *index = malloc(sizeof(int));
        *index = i + 1;
        pthread_create(&consumers[i], NULL, consume, index);
    }

    //5. Sleep
    sleep(timeSleep);

    //6. Exit
    pthread_mutex_destroy(&mutex);
    sem_destroy(empty);
    sem_destroy(full);
    printf("Exiting...");
    return 0;
}
