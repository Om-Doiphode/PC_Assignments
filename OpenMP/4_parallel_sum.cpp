#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);
    int *arr = new int[n];
    double t1, t2;
    for (int i = 0; i < n; i++)
        arr[i] = i + 1;

    long long sum = 0;
    t1 = omp_get_wtime();
#pragma omp parallel for reduction(+ : sum)
    for (int i = 0; i < n; i++)
        sum += arr[i];
    t2 = omp_get_wtime();
    cout << "Parallel Sum: " << sum << endl;
    cout << "Time: " << t2 - t1 << endl;

    sum=0;
    t1 = omp_get_wtime();
    for (int i = 0; i < n; i++)
        sum += arr[i];
    t2 = omp_get_wtime();

    cout << "\nSerial Sum: " << sum << endl;
    cout << "Time: " << t2 - t1 << endl;
    return 0;
}