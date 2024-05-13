#include <stdio.h>
#include <omp.h>
#include <limits.h>
#include <stdlib.h>
#define ARR_SIZE 1000000

// Maximum Subarray sum
int maxSubArray(int *nums)
{
    int maxi = INT_MIN;
    int sum = 0;

    for (int i = 0; i < ARR_SIZE; i++)
    {
        sum += nums[i];

        if (sum > maxi)
            maxi = sum;

        if (sum < 0)
            sum = 0;
    }

    return maxi;
}
int main()
{
    // int arr[] = {5,4,-1,7,8};
    int arr[ARR_SIZE];
    int i, tid, numt, sum = 0;
    int maxi = INT_MIN;
    double t1, t2;

    // Initialize array with random values
    for (i = 0; i < ARR_SIZE; i++)
    {
        arr[i] = rand() % 100 - 60; // Random numbers between -50 and 49
    }

    t1 = omp_get_wtime();
    int ans = maxSubArray(arr);
    t2 = omp_get_wtime();

    printf("Serial code\n");
    printf("Max sum: %d\n", ans);
    printf("Time taken: %f\n", t2 - t1);
    printf("______________________________\n");

    t1 = omp_get_wtime();
#pragma omp parallel default(shared) private(i)
    {
        int psum = 0;

#pragma omp for
        for (i = 0; i < ARR_SIZE; i++)
        {
            psum += arr[i];
            if (psum > maxi)
                maxi = psum;

            if (psum < 0)
                psum = 0;
        }

#pragma omp critical
        sum += psum;
    }
    t2 = omp_get_wtime();

    printf("\nParallel code\n");
    printf("Max sum: %d\n", maxi);
    printf("Time taken: %f\n", t2 - t1);

    return 0;
}