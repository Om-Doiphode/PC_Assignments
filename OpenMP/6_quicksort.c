#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#define ARR_SIZE 900000
int partition(int arr[], int low, int high)
{
    // Your code here
    int pivot = arr[low];
    int i = low;
    int j = high;

    while (i < j)
    {
        while (arr[i] <= pivot && i <= high - 1)
        {
            i++;
        }
        while (arr[j] > pivot && j >= low + 1)
        {
            j--;
        }

        if (i < j)
        {
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    int temp = arr[low];
    arr[low] = arr[j];
    arr[j] = temp;
    return j;
}
void parallelQuickSort(int arr[], int low, int high)
{
    // code here
    int partitionIndex;
    if (low < high)
    {
        partitionIndex = partition(arr, low, high);
#pragma omp parallel sections
        {
#pragma omp section
            {
                parallelQuickSort(arr, low, partitionIndex - 1);
            }
#pragma omp section
            {
                parallelQuickSort(arr, partitionIndex + 1, high);
            }
        }
    }
}

void serialQuickSort(int arr[], int low, int high)
{
    // code here
    int partitionIndex;
    if (low < high)
    {
        partitionIndex = partition(arr, low, high);
        serialQuickSort(arr, low, partitionIndex - 1);
        serialQuickSort(arr, partitionIndex + 1, high);
    }
}
int main()
{
    // int arr[] = {5, 4, -1, 7, 8};
    int arr1[ARR_SIZE], arr2[ARR_SIZE];
    int i;
    double t1, t2;
    // Initialize array with random values
    for (i = 0; i < ARR_SIZE; i++)
    {
        arr1[i] = rand() % 100 - 50; // Random numbers between -50 and 49
        arr2[i] = arr1[i];
    }

    t1 = omp_get_wtime();
    parallelQuickSort(arr1, 0, ARR_SIZE - 1);
    t2 = omp_get_wtime();

    printf("Sorted array: ");
    for (i = 0; i < ARR_SIZE; i++)
    {
        printf("%d ", arr1[i]);
    }
    printf("\n-------------------------------------------------");
    printf("\n\nParallel Quicksort\n");
    printf("Time taken: %f\n", t2 - t1);

    t1 = omp_get_wtime();
    serialQuickSort(arr2, 0, ARR_SIZE - 1);
    t2 = omp_get_wtime();

    printf("\n Serial Quicksort \n");
    printf("Time taken: %f\n", t2 - t1);
    return 0;
}