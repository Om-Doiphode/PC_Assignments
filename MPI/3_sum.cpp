#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;
int main(int argc, char *argv[]) {
    int rank, size;
    long long n = atoi(argv[1]);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long start = (n / size) * rank;
    long long end = (rank == size - 1) ? n : (n / size) * (rank + 1);

    long long partial_sum = 0;
    for (long long i = start; i < end; i++) {
        partial_sum += i+1;
    }

    long long total_sum;
    MPI_Reduce(&partial_sum, &total_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        cout << "Total sum: " << total_sum << endl;
    }

    MPI_Finalize();

    return 0;
}
