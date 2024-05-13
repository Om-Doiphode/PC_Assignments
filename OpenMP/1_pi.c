#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <num_steps> <num_threads>\n", argv[0]);
        return 1;
    }

    int num_steps = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    double pi, step = 1.0 / (double)num_steps, x = 0.0, sum = 0.0;
    double t1,t2;

    // Set the number of threads
    omp_set_num_threads(num_threads);
    t1=omp_get_wtime();
#pragma omp parallel private(x) shared(sum)
    {
#pragma omp for reduction(+:sum) schedule(static)
        for (int i = 0; i < num_steps; i++) {
            x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
    }

    pi = step * sum;
    t2=omp_get_wtime();
    printf("The Value of PI is %lf\n", pi);
    printf("Time: %lf\n", t2-t1);
    return 0;
}
