#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
    int n, myid, numprocs, i;
    double PI25DT = 3.141592653589793238462643;
    double mypi, pi, h, sum, x;
    double start_time, end_time, execution_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    if (myid == 0) {
        if (argc < 2) {
            printf("Usage: %s <n>\n", argv[0]);
            MPI_Finalize();
            return 1;
        }
        n = atoi(argv[1]);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (n == 0) {
        MPI_Finalize();
        return 0;
    }
    else {
        h = 1.0 / (double)n;
        sum = 0.0;
        start_time = MPI_Wtime(); 

        for (i = myid + 1; i <= n; i += numprocs) {
            x = h * ((double)i - 0.5);
            sum += (4.0 / (1.0 + x * x));
        }

        mypi = h * sum;

        MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if (myid == 0) {
            end_time = MPI_Wtime(); 
            execution_time = end_time - start_time;
            printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
            printf("Execution Time: %lf seconds\n", execution_time);
        }
    }

    MPI_Finalize();
    return 0;
}
