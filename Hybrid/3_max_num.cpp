#include <iostream>
#include <mpi.h>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <omp.h>

#define ARRAY_SIZE 100000000
using namespace std;

// Commands:
// mpic++ 3_max_num.cpp -o max -fopenmp
// mpirun -np 8 ./max

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double start_time = MPI_Wtime();
    int* data = new int[ARRAY_SIZE];
    srand(time(NULL)); 
    #pragma omp parallel for
    for (int i = 0; i < ARRAY_SIZE; ++i) {
        data[i] = rand()%ARRAY_SIZE;
    }

    int local_max = numeric_limits<int>::min();
    #pragma omp parallel reduction(max:local_max)
    {
        int tid = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        int local_start = (rank * (ARRAY_SIZE / size)) + (tid * (ARRAY_SIZE / size / num_threads));
        int local_end = (rank == size - 1) ? ARRAY_SIZE : ((rank + 1) * (ARRAY_SIZE / size));
        for (int i = local_start; i < local_end; ++i) {
            if (data[i] > local_max) {
                local_max = data[i];
            }
        }
    }

    int global_max;
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double end_time = MPI_Wtime();
        double elapsed_time = end_time - start_time;
        cout << "Maximum value in the array is: " << global_max << endl;
        cout << "Elapsed time: " << elapsed_time << " seconds" << endl;
    }

    delete[] data;

    MPI_Finalize();
    return 0;
}
