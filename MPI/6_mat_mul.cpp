#include <iostream>
#include <vector>
#include <cstdlib>
#include <mpi.h>

#define NRA 3
#define NCA 3
#define NCB 3
#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2

using namespace std;

int main(int argc, char *argv[]) {
    int numtasks, taskid, numworkers, source, dest, mtype, rows, averow, extra, offset, i, j, k, rc;
    double a[NRA][NCA], b[NCA][NCB], c[NRA][NCB];
    MPI_Status status;

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
        cout << "mpi_mm has started with " << numtasks << " tasks." << endl;
        cout << "Initializing arrays..." << endl;

        int k=1;
        // Initialize matrices A and B
        for (i = 0; i < NRA; i++) {
            for (j = 0; j < NCA; j++) {
                a[i][j] = k++;
            }
        }
        k=1;
        for (i = 0; i < NCA; i++) {
            for (j = 0; j < NCB; j++) {
                b[i][j] = k++;
            }
        }

        // Send matrix data to the worker tasks
        averow = NRA / numworkers;
        extra = NRA % numworkers;
        offset = 0;
        mtype = FROM_MASTER;

        for (dest = 1; dest <= numworkers; dest++) {
            rows = (dest <= extra) ? averow + 1 : averow;
            cout << "Sending " << rows << " rows to task " << dest << " offset=" << offset << endl;
            
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
            cout << "Received results from task " << source << endl;
        }

        // Print the resultant matrix C
        cout << "Resultant Matrix C:" << endl;
        for (i = 0; i < NRA; i++) {
            for (j = 0; j < NCB; j++) {
                cout << c[i][j] << " ";
            }
            cout << endl;
        }
    } else if (taskid > MASTER) {
        mtype = FROM_MASTER;
        MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&a, rows * NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&b, NCA * NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

        for (k = 0; k < NCB; k++) {
            for (i = 0; i < rows; i++) {
                c[i][k] = 0.0;
                for (j = 0; j < NCA; j++) {
                    c[i][k] += a[i][j] * b[j][k];
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
