#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <omp.h>

#define BUFFER_SIZE 10
#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 2
#define NUM_NUMBERS 20

std::queue<int> buffer;
bool done_producing = false;
int numbers_produced = 0;

void producer() {
    int producer_id = omp_get_thread_num();
    srand(time(0));

    while (numbers_produced < NUM_NUMBERS) {
        #pragma omp critical(buffer_update)
        {
            if (buffer.size() >= BUFFER_SIZE) {
                continue;
            }

            int number = rand() % 100;  // Generate random number
            buffer.push(number);
            std::cout << "Producer " << producer_id << " produced: " << number << std::endl;

            numbers_produced++;
        }
    }
}

void consumer() {
    int consumer_id = omp_get_thread_num();

    while (true) {
        int number = 0;
        bool can_consume = false;

        #pragma omp critical(buffer_update)
        {
            if (!buffer.empty()) {
                number = buffer.front();
                buffer.pop();
                can_consume = true;
            }
            else if (done_producing) {
                break;
            }
        }

        if (can_consume) {
            std::cout << "Consumer " << consumer_id << " consumed: " << number << std::endl;
        }
    }
}

int main() {
    srand(time(0));

    #pragma omp parallel sections num_threads(NUM_PRODUCERS + NUM_CONSUMERS)
    {
        #pragma omp section
        {
            for (int i = 0; i < NUM_PRODUCERS; i++) {
                producer();
            }
        }

        #pragma omp section
        {
            for (int i = 0; i < NUM_CONSUMERS; i++) {
                consumer();
            }
        }
    }

    return 0;
}
