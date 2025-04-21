#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mpi.h"

#define TOKEN 1
#define NO_TOKEN 0
#define REQUEST_PROBABILITY 1 // Always request access in this simple demo
int main(int argc, char** argv) {
    int rank, size;
    int has_token = 0;
    int request_print = 0;
    int token;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Rank of current process
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Total number of processes
    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;
    MPI_Status status;

    // Let process 0 start with the token
    if (rank == 0) {
        has_token = 1;
        token = TOKEN;
        printf("Process %d starts with the token.\n", rank);
    }

    for (int round = 0; round < 5; round++) { // Run 5 token rounds
        // Receive token if not currently holding it
        if (!has_token) {
            MPI_Recv(&token, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
            has_token = 1;
        }

        // Simulate a request to print
        request_print = REQUEST_PROBABILITY;

        if (has_token && request_print) {
            // Simulate printing
            printf("Process %d is printing (round %d)...\n", rank, round + 1);
            sleep(1); // Simulate time taken to print
        }

        // Pass token to next process
        MPI_Send(&token, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        has_token = 0;
    }

    MPI_Finalize();
    return 0;
}
