# Producer-Consumer Problem with MPI, OpenMP, and Pthreads
This repository contains implementations of the classic producer-consumer problem using different parallel programming paradigms: MPI (Message Passing Interface), OpenMP, and Pthreads. The producer-consumer problem involves coordinating the interaction between producer processes that generate data and consumer processes that consume the data. The goal is to ensure that producers and consumers can work concurrently and efficiently without data race conditions or deadlocks.

## MPI Implementation
The MPI implementation of the producer-consumer problem uses message passing to exchange data between processes. Multiple producer processes generate random numbers and send them to consumer processes through message passing. The consumer processes receive the numbers and consume them from a shared buffer. The implementation ensures proper synchronization and avoids conflicts between producers and consumers.

## OpenMP Implementation
The OpenMP implementation leverages shared memory parallelism to enable multiple threads to work concurrently on the producer-consumer problem. The implementation uses OpenMP directives to parallelize the producer and consumer tasks. The producers generate random numbers and add them to a shared buffer, while the consumers retrieve numbers from the buffer. Proper synchronization mechanisms, such as locks or critical sections, are used to ensure data integrity and prevent race conditions.

## Pthreads Implementation
The Pthreads implementation utilizes POSIX threads to achieve parallelism in the producer-consumer problem. Multiple threads act as producers and consumers, where each thread generates random numbers or consumes them from a shared buffer. The implementation employs thread synchronization constructs, such as mutexes or condition variables, to coordinate access to the shared buffer and prevent data races.

## Repository Structure
The repository is organized as follows:
- mpi/
    - producer_consumer_mpi.c  // MPI implementation of the producer-consumer problem
- openmp/
    - producer_consumer_openmp.c  // OpenMP implementation of the producer-consumer problem
- pthreads/
    - producer_consumer_pthreads.c  // Pthreads implementation of the producer-consumer problem
- README.md  // This README file
# Getting Started
To compile and run the implementations, follow these steps:
* Clone this repository:
* Compile the source code using the appropriate compiler and flags. For example:

  * MPI: mpicc -o producer_consumer_mpi producer_consumer_mpi.c
  * OpenMP: gcc -fopenmp -o producer_consumer_openmp producer_consumer_openmp.c
  * Pthreads: gcc -pthread -o producer_consumer_pthreads producer_consumer_pthreads.c
*Run the compiled executable:

  * MPI: mpirun -np <num_processes> ./producer_consumer_mpi
  * OpenMP: ./producer_consumer_openmp
  * Pthreads: ./producer_consumer_pthreads
## Notes
Adjust the number of producer and consumer processes/threads as needed for your specific scenario by modifying the respective variables or command-line arguments in the source code.
The implementations provided in this repository serve as examples and may require further modifications or enhancements based on your specific requirements.
## References
"An Introduction to Parallel Programming" - PETER PACHECO
