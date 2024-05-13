#include <iostream>
#include <vector>
#include <cstdlib>
#include <mpi.h>

#define NRA 500
#define NCA 500
#define NCB 500
#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

using namespace std;

// Commands:
// mpic++ 1_mat_mul.cpp -o mat_mul -fopenmp
// mpirun -np 8 ./mat_mul

int main(int argc, char *argv[]) {
    int numtasks, taskid, numworkers, source, dest, mtype, rows, averow, extra, offset, i, j, k, rc;
    double a[NRA][NCA], b[NCA][NCB], c[NRA][NCB],d[NRA][NCB];
    MPI_Status status;
    double start_time, end_time;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    if (numtasks < 2) {
        cout << "Need at least two MPI tasks. Quitting..." << endl;
        MPI_Abort(MPI_COMM_WORLD, rc);
        exit(1);
    }

    numworkers = numtasks - 1;

    if (taskid == MASTER) {
        // cout << "mpi_mm has started with " << numtasks << " tasks." << endl;
        // cout << "Initializing arrays..." << endl;

        int k=1;
        // Initialize matrices A and B
        for (i = 0; i < NRA; i++) {
            for (j = 0; j < NCA; j++) {
                a[i][j] = rand()%1000;
            }
        }
        k=1;
        for (i = 0; i < NCA; i++) {
            for (j = 0; j < NCB; j++) {
                b[i][j] = rand()%1000;
            }
        }

          start_time = MPI_Wtime();
        
        for (i = 0; i < NRA; i++) {
            for (j = 0; j < NCB; j++) {
                d[i][j] = 0.0;
                for (k = 0; k < NCA; k++) {
                    d[i][j] += a[i][k] * b[k][j];
                }
            }
        }

        end_time = MPI_Wtime();
        cout << "Serial Execution time: " << end_time - start_time << " seconds" << endl;


        // Send matrix data to the worker tasks
        averow = NRA / numworkers;
        extra = NRA % numworkers;
        offset = 0;
        mtype = FROM_MASTER;

         start_time = MPI_Wtime(); // Start timing


        for (dest = 1; dest <= numworkers; dest++) {
            rows = (dest <= extra) ? averow + 1 : averow;
            // cout << "Sending " << rows << " rows to task " << dest << " offset=" << offset << endl;
            
            MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&a[offset][0], rows * NCA, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&b, NCA * NCB, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
            offset += rows;
        }

        // Receive results from worker tasks
        mtype = FROM_WORKER;

        for (i = 1; i <= numworkers; i++) {
            source = i;
            MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&c[offset][0], rows * NCB, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
            // cout << "Received results from task " << source << endl;
        }

         end_time = MPI_Wtime();

        // Print the execution time
        cout << "Parallel Execution time: " << end_time - start_time << " seconds" << endl;

        // Print the resultant matrix C
        // cout << "Resultant Matrix C:" << endl;
        // for (i = 0; i < NRA; i++) {
        //     for (j = 0; j < NCB; j++) {
        //         cout << c[i][j] << " ";
        //     }
        //     cout << endl;
        // }
    } else if (taskid > MASTER) {
        mtype = FROM_MASTER;
        MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, rows * NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, NCA * NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

    #pragma omp parallel shared(a,b,c) private(i,j,k)
    {
        #pragma omp for schedule(static)
        for (k = 0; k < NCB; k++) {
            for (i = 0; i < rows; i++) {
                c[i][k] = 0.0;
                for (j = 0; j < NCA; j++) {
                    c[i][k] += a[i][j] * b[j][k];
                }
            }
        }
    }

        mtype = FROM_WORKER;
        MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&c, rows * NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
