#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;

// Commands:
// mpic++ -std=c++14 -o 2_rank MPI/2_rank.cpp
// mpirun -np 8 ./2_rank
int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    cout << "Process " << rank << " out of " << size << " processes" << endl;

    MPI_Finalize();

    return 0;
}
