#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;
int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    vector<int> array = {3, 8, 1, 6, 2, 9, 4, 5, 7};
    int local_size = array.size() / size;
    int remainder = array.size() % size;
    int start = rank * local_size;
    int end = start + local_size;
    if (rank == size - 1) {
        end += remainder; 
    }
    int local_max = numeric_limits<int>::min();
    for (int i = start; i < end; ++i) {
        local_max = max(local_max, array[i]);
    }
    int global_max;
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        cout << "The maximum element in the array is: " << global_max << endl;
    }

    MPI_Finalize();
    return 0;
}
