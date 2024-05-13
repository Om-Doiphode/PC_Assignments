#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>

const int M = 1000;
const int P = 1000;
const int N = 1000;

using namespace std;

void printMatrix(const vector<vector<float>> &matrix) {
    for (const auto &row : matrix) {
        for (const auto &elem : row) {
            cout << elem << " ";
        }
        cout << endl;
    }
}

int main() {
    vector<vector<float>> A(N, vector<float>(P));
    vector<vector<float>> B(P, vector<float>(M));
    vector<vector<float>> C(N, vector<float>(M, 0)); // Initialize C with zeros

    unsigned long i, j, k;

    // Initialize matrices A and B with random values
    for (i = 0; i < N; i++) {
        for (k = 0; k < P; k++) {
            A[i][k] = rand();
        }
    }

    for (k = 0; k < P; k++) {
        for (j = 0; j < M; j++) {
            B[k][j] = rand();
        }
    }

    // Perform matrix multiplication using OpenMP
#pragma omp parallel shared(A, B, C) private(i, j, k)
    {
#pragma omp for schedule(static)
        for (i = 0; i < N; i++) {
            for (j = 0; j < M; j++) {
                for (k = 0; k < P; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }

    // Print the resultant matrix C
    cout << "Matrix C:" << endl;
    printMatrix(C);

    return 0;
}
