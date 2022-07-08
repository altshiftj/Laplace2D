#include <iostream>
#include <stdio.h>

#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Status status;
    int my_id, num_procs;
    double tmp;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    double my_old_data = my_id * my_id;
    double my_new_data;

    int destination = my_id - 1;
    if (destination == -1)
        destination = num_procs - 1; //... each process sends to its right neighbor
    int source = my_id + 1;
    if (source == num_procs)
        source = 0; //... each process receives from its left neighbor

    // sending and receiving
    MPI_Send(&my_old_data, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD);
    MPI_Recv(&my_new_data, 1, MPI_DOUBLE, destination, 0, MPI_COMM_WORLD, &status);

    // At the end process 0 is collecting and printing the data
    if (my_id == 0)
    {
        double *a = new double[num_procs];
        a[0] = my_new_data;
        for (int p = 1; p < num_procs; p++)
        {
            MPI_Recv(&tmp,
                     1,
                     MPI_DOUBLE,
                     p,
                     0,
                     MPI_COMM_WORLD,
                     &status);

            a[p] = tmp;
        }
        for (int p = 0; p < num_procs; p++)
            std::cout << "a[" << p << "] = " << a[p] << std::endl;
        delete[] a;
    }
    else
    {
        MPI_Send(&my_new_data,
                 1,
                 MPI_DOUBLE,
                 0,
                 0,
                 MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return (0);
}
