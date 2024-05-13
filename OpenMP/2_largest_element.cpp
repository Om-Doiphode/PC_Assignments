#include <iostream>
#include <vector>
#include <omp.h>

// Commands:
//  g++ 2_largest_element.cpp -o large -fopenmp
//  ./large <number of elements>
using namespace std;
int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);
    int numOfThreads=atoi(argv[2]);
    vector<int> a(n);

    // Fill the vector a with random numbers
    srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < n; i++)
    {
        a[i] = rand() % 1000;
        if (rand() % 2)
        {
            a[i] = -a[i];
        }
    }

    int max = a[0];
    omp_set_num_threads(numOfThreads);
    // Measure execution time
    double t1 = omp_get_wtime();

// Parallel region with reduction
#pragma omp parallel
    {
        int local_max = max; // Each thread has its own local_max

// Parallel loop with reduction
#pragma omp for
        for (int i = 1; i < n; i++)
        {
            if (a[i] > local_max)
            {
                local_max = a[i];
            }
        }

// Combine local_max values using reduction
#pragma omp critical
        {
            if (local_max > max)
            {
                max = local_max;
            }
        }
    }

    // End of parallel region

    // Measure and print execution time
    double t2 = omp_get_wtime();
    cout << "Max number: " << max << endl;
    cout << "Execution time: " << (t2 - t1) << " seconds" << endl;

    return 0;
}
