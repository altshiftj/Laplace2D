#include <stdio.h>
#include <iostream>
#include <chrono>
#include <unistd.h>

#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int my_process_id;
    int number_of_processes;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_process_id);
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);

    const auto start_chrono = std::chrono::high_resolution_clock::now();

    // sending / receiving
  


    if (my_process_id == 0)
    {
        const auto finish_chrono = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = finish_chrono - start_chrono;
        std::cout << "Runtime: " << diff.count() << " s. " << std::endl;
    }

    MPI_Finalize();
    return 0;
}

// standard communication (blocking & asynchron): 9 sec
// non blocking communication asynchron: 9 sec
// sychronous communication: 11 sec