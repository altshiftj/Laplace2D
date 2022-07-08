#include <iostream>

#include <mpi.h>


void primary(int number_of_processes);
void secondary(int my_process_id);

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int my_process_id;
    int number_of_processes;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_process_id);
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);


    if (number_of_processes < 2)
    {
        printf("Error: cannot execute with %d processors!\n", number_of_processes);

        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    if (my_process_id == 0)
    {
        printf("Lets get started...\n");
        primary(number_of_processes);
    }
    else
        secondary(my_process_id);
    

    MPI_Finalize();
    return 0;
}

const int sending_size = 256;

void primary(int number_of_processes)
{
    char message[sending_size];
    MPI_Status status;

    for(int source_process_id = 1; source_process_id < number_of_processes; source_process_id++)
    {
        MPI_Recv(message, sending_size, MPI_CHAR, source_process_id, 0, MPI_COMM_WORLD, &status);
        std::cout << message;
        MPI_Recv(message, sending_size, MPI_CHAR, source_process_id, 1, MPI_COMM_WORLD, &status); 
        std::cout << message;
    }
}

void secondary(int my_process_id)
{
    char message[sending_size];

    sprintf(message, "Hello from %d\n", my_process_id);

    const int destination_process_id = 0;
    const int tag_1 = 0;
    MPI_Send(message, sending_size, MPI_CHAR, destination_process_id, tag_1, MPI_COMM_WORLD);

    // We can reuse the message, because we use the blocked version!!!!
    const int tag_2 = 1;
    sprintf(message, "Goodbye from %d\n", my_process_id);
    MPI_Send(message, sending_size, MPI_CHAR, destination_process_id, tag_2, MPI_COMM_WORLD);
}
