#include <iostream>
#include <mpi.h>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <omp.h>

#define ARRAY_SIZE 100000000

// Commands:
// mpic++ 2_min_num.cpp -o min -fopenmp
// mpirun -np 8 ./min
using namespace std;
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
        data[i] = rand();
    }

    int local_min = numeric_limits<int>::max();
    #pragma omp parallel reduction(min:local_min)
    {
        int tid = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        int local_start = (rank * (ARRAY_SIZE / size)) + (tid * (ARRAY_SIZE / size / num_threads));
        int local_end = (rank == size - 1) ? ARRAY_SIZE : ((rank + 1) * (ARRAY_SIZE / size));
        for (int i = local_start; i < local_end; ++i) {
            if (data[i] < local_min) {
                local_min = data[i];
            }
        }
    }

    int global_min;
    MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double end_time = MPI_Wtime();
        double elapsed_time = end_time - start_time;
        cout << "Minimum value in the array is: " << global_min << endl;
        cout << "Elapsed time: " << elapsed_time << " seconds" << endl;
    }

    delete[] data;

    MPI_Finalize();
    return 0;
}
