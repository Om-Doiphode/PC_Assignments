#include <iostream>
#include <omp.h>
using namespace std;
long long parallelFactorial(int n)
{
    if (n <= 1)
        return 1;

    long long result = 1;

#pragma omp parallel for reduction(* : result)
    for (int i = 2; i <= n; i++)
    {
        result *= i;
    }

    return result;
}

long long serialFactorial(int n)
{
    if (n <= 1)
        return 1;

    long long result = 1;
    for (int i = 2; i <= n; i++)
    {
        result *= i;
    }

    return result;
}

int main(int argc, char *argv[])
{
    int n=atoi(argv[1]);
    double start_time, end_time;

    cout<<"Parallel Factorial: "<<endl;
    start_time = omp_get_wtime();
    cout << parallelFactorial(n) << endl;
    end_time = omp_get_wtime();
    cout<<"Time: "<<end_time-start_time<<endl;

    cout<<"\nSerial Factorial: "<<endl;
    start_time = omp_get_wtime();
    cout<<serialFactorial(n)<<endl;
    end_time = omp_get_wtime();
    cout<<"Time: "<<end_time-start_time<<endl;
    return 0;
}
