#include <iostream>
#include <chrono>
#include <cmath>

#include <mpi.h>

#define PI 3.14159265358979323846  /* pi */

double integral(double boundaryLeft, double boundaryRight, long divisions);
double f(double x);

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int number_of_processes;
    int my_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    const int repetitions = 2;

    double result = 0.0;
    double local_integral = 0.0;
    const auto start_chrono = std::chrono::high_resolution_clock::now();

    const long number_of_intervals = 5040 * 100000;

    const double global_a = 0.0;
    const double global_b = 1.0;

    for (int round = 0; round < repetitions; round++)
    {

        const long local_number_of_intervals = number_of_intervals / number_of_processes;
        const double local_interval_width = (global_b - global_a) / (double)number_of_processes;
        const double local_a = global_a + my_rank * local_interval_width;
        const double local_b = local_a + local_interval_width;

        local_integral = integral(local_a, local_b, local_number_of_intervals);
        // result = local_integral;
        
        // int tag = 0;
        // if (my_rank == 0)
        // {
            
        //     double receive_buffer;
        //     MPI_Status status;
        //     for (int source=1; source<number_of_processes; source++) {
                
        //         MPI_Recv(&receive_buffer,1,MPI_DOUBLE,source,tag,MPI_COMM_WORLD, &status);
        //         result+= receive_buffer;
        //     }
            
        // }
        // else {
        //     int destination_id =0;
            
        //     MPI_Ssend( &local_integral, 1, MPI_DOUBLE, destination_id, tag, MPI_COMM_WORLD);
        // }

        MPI_Allreduce(&local_integral, &result, 1, MPI_DOUBLE, MPI_SUM ,MPI_COMM_WORLD);
        

    }

    //result = result / repetitions;

    const auto finish_chrono = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = finish_chrono - start_chrono;
    if (my_rank == 0)
    {
        std::cout << "Runtime: " << diff.count() << " s. " << std::endl;
        printf("Result: %0.20f \n", result * 4.);
        printf("Error: %0.20f \n", fabs(result * 4 - PI));
    }

    MPI_Finalize();
    return 0;
}

double integral(double boundaryLeft, double boundaryRight, long divisions)
{
    double tmp = 0.0;
    double locIntervalWidth = (boundaryRight - boundaryLeft) / (double)divisions;
    for (int i = 0; i < divisions; i++)
    {
        tmp += f(boundaryLeft + locIntervalWidth * ((double)i + 0.5)) * locIntervalWidth;
    }
    return tmp;
}

double f(double x)
{
    return (std::sqrt(1. - x * x));
}
