#include <bits/stdc++.h>
#include <mpi.h>

using namespace std;
int prime_number(int n, int id, int p)
{
    int i;
    int j;
    int prime;
    int total;

    total = 0;

    for (i = 2 + id; i <= n; i = i + p)
    {
        prime = 1;
        for (j = 2; j < i; j++)
        {
            if ((i % j) == 0)
            {
                prime = 0;
                break;
            }
        }
        total = total + prime;
    }
    return total;
}


int main(int argc, char *argv[])

{
    int id;
    int ierr;
    int n;
    int p;
    int primes;
    int primes_part;
    double wtime;

    ierr = MPI_Init(&argc, &argv);

    if (ierr != 0)
    {
        cout << "\n";
        cout << "PRIME_MPI - Fatal error!\n";
        cout << "  MPI_Init returned nonzero ierr.\n";
        exit(1);
    }

    ierr = MPI_Comm_size(MPI_COMM_WORLD, &p);

    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &id);

    if (id == 0)
    {
        n = atoi(argv[1]);
        cout << "\n";
        cout << "         N        Pi          Time\n";
        cout << "\n";
    }

    if (id == 0)
    {
        wtime = MPI_Wtime();
    }
    ierr = MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    primes_part = prime_number(n, id, p);

    ierr = MPI_Reduce(&primes_part, &primes, 1, MPI_INT, MPI_SUM, 0,
                      MPI_COMM_WORLD);

    if (id == 0)
    {
        wtime = MPI_Wtime() - wtime;

        cout << "  " << setw(8) << n
             << "  " << setw(8) << primes
             << "  " << setw(14) << wtime << "\n";
    }

    MPI_Finalize();

    return 0;
}