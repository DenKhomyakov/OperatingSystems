#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

#define TRUE 1
#define FALSE 0

int ready = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condVar = PTHREAD_COND_INITIALIZER;

using std::cout;
using std::endl;

void producerUp() {
    pthread_mutex_lock(&mutex);

    if (ready == 1) {
        pthread_mutex_unlock(&mutex);

        return;
    }

    ready = 1;
    cout << "Provided" << endl;

    pthread_cond_signal(&condVar);
    pthread_mutex_unlock(&mutex);
}

void consumerUp() {
    pthread_mutex_lock(&mutex);

    while (ready == 0) {
        pthread_cond_wait(&condVar, &mutex);
        cout << "Awoke" << endl;
    }

    ready = 0;
    cout << "Consumed" << endl;

    pthread_mutex_unlock(&mutex);
}

void* producer(void* arg) {
    while (TRUE) {
        producerUp();
    }

    return NULL;
}

void* consumer(void* arg) {
    while (TRUE) {
        consumerUp();
    }

    return NULL;
}

int main() {
    pthread_t producerThread;
    pthread_t consumerThread;

    if (pthread_create(&producerThread, NULL, producer, NULL) != 0) {
        perror("Failed to create producer thread!");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&consumerThread, NULL, consumer, NULL) != 0) {
        perror("Failed to create consumer thread!");
        exit(EXIT_FAILURE);
    }

    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    return 0;
}
