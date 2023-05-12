#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

#define BUFFER_SIZE 10
#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 2
#define NUM_NUMBERS 20

std::queue<int> buffer;
bool done_producing = false;
int numbers_produced = 0;

void producer(int rank) {
    srand(time(0) + rank);

    while (numbers_produced < NUM_NUMBERS) {
        if (buffer.size() >= BUFFER_SIZE) {
            continue;
        }

        int number = rand() % 100;  // Generate random number
        buffer.push(number);
        std::cout << "Producer " << rank << " produced: " << number << std::endl;

        numbers_produced++;

        // Send signal to consumers
        MPI_Send(&number, 1, MPI_INT, NUM_PRODUCERS, 0, MPI_COMM_WORLD);
    }
}

void consumer(int rank) {
    while (true) {
        int number = 0;

        // Receive signal from producers
        MPI_Recv(&number, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        bool can_consume = false;

        if (!buffer.empty()) {
            number = buffer.front();
            buffer.pop();
            can_consume = true;
        }
        else if (done_producing) {
            break;
        }

        if (can_consume) {
            std::cout << "Consumer " << rank << " consumed: " << number << std::endl;
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank < NUM_PRODUCERS) {
        producer(rank);
    }
    else {
        consumer(rank - NUM_PRODUCERS);
    }

    // Signal end of producing
    if (rank < NUM_PRODUCERS) {
        done_producing = true;
        for (int i = NUM_PRODUCERS; i < size; i++) {
            MPI_Send(NULL, 0, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();

    return 0;
}
