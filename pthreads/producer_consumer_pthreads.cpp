#include <pthread.h>
#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>

#define BUFFER_SIZE 10
#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 2
#define NUM_NUMBERS 20

pthread_mutex_t buffer_mutex;
pthread_cond_t buffer_not_full;
pthread_cond_t buffer_not_empty;
std::queue<int> buffer;

bool done_producing = false;
int numbers_produced = 0;

void* producer(void* arg) {
    int producer_id = *(int*)arg;
    srand(time(0));

    while (numbers_produced < NUM_NUMBERS) {
        pthread_mutex_lock(&buffer_mutex);

        while (buffer.size() >= BUFFER_SIZE) {
            pthread_cond_wait(&buffer_not_full, &buffer_mutex);
        }

        int number = rand() % 100;  // Generate random number
        buffer.push(number);
        std::cout << "Producer " << producer_id << " produced: " << number << std::endl;

        numbers_produced++;
        pthread_cond_signal(&buffer_not_empty);
        pthread_mutex_unlock(&buffer_mutex);
    }

    pthread_exit(NULL);
}

void* consumer(void* arg) {
    int consumer_id = *(int*)arg;

    while (true) {
        pthread_mutex_lock(&buffer_mutex);

        while (buffer.empty() && !done_producing) {
            pthread_cond_wait(&buffer_not_empty, &buffer_mutex);
        }

        if (buffer.empty() && done_producing) {
            pthread_mutex_unlock(&buffer_mutex);
            break;
        }

        int number = buffer.front();
        buffer.pop();
        std::cout << "Consumer " << consumer_id << " consumed: " << number << std::endl;

        pthread_cond_signal(&buffer_not_full);
        pthread_mutex_unlock(&buffer_mutex);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int producer_ids[NUM_PRODUCERS];
    int consumer_ids[NUM_CONSUMERS];

    pthread_mutex_init(&buffer_mutex, NULL);
    pthread_cond_init(&buffer_not_full, NULL);
    pthread_cond_init(&buffer_not_empty, NULL);

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producer_ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, (void*)&producer_ids[i]);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumer_ids[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, (void*)&consumer_ids[i]);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }

    done_producing = true;
    pthread_cond_broadcast(&buffer_not_empty);

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&buffer_mutex);
    pthread_cond_destroy(&buffer_not_full);
    pthread_cond_destroy(&buffer_not_empty);

    return 0;
}
