#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc, char **argv) 
{
    int rank, size, tag = 0;
    MPI_Status status;
    char send_msg[50], recv_msg[50];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);     // Get current process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);     // Get total number of processes
    // Define the predecessor and successor in the ring
    int successor = (rank + 1) % size;
    int predecessor = (rank - 1 + size) % size;
    // Prepare the message to send
    sprintf(send_msg, "Hello from process %d", rank);
    // Send message to successor and receive from predecessor
    MPI_Sendrecv(send_msg, strlen(send_msg)+1, MPI_CHAR, successor, tag,
                 recv_msg, 50, MPI_CHAR, predecessor, tag,
                 MPI_COMM_WORLD, &status);
    // Print the message received
    printf("Process %d received message: '%s'\n", rank, recv_msg);
    MPI_Finalize();
    return 0;
}
