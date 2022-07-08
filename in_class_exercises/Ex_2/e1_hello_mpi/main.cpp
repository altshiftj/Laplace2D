#include <iostream>
#include <stdio.h>

// including mpi header to get access to the mpi functions
#include <mpi.h>

int main(int argc, char **argv)
{
    // Initialization of MPI
    MPI_Init(&argc, &argv);

    int my_process_id;
    int number_of_processes;

    /*
    * Processes are combined into groups.
    * MPI_COMM_WORLD : World Communicator includes all processes.
    */

    // get the my process ID (rank) 
    MPI_Comm_rank(MPI_COMM_WORLD, &my_process_id);

    // get the number of processes 
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);

    printf("%d of %d is here\n", my_process_id, number_of_processes);
    //std::cout << my_process_id << " of " << number_of_processes << " is here!\n";

    // Finalization of MPI
    MPI_Finalize();
    return 0;
}
