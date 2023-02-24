// Copyright 2022 Davydov Andrey
#include <mpi.h>
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include "../../../modules/task_2/davydov_a_jacobi/jacobi.h"

float normDifferent(std::vector<float> x, std::vector<float> y) {
    float norm = std::abs(x[0] - y[0]);
    const int size = static_cast<int>(x.size());
    for (int i = 1; i < size; ++i) {
        float iterationNorm = std::abs(x[i] - y[i]);
        if (iterationNorm > norm) {
            norm = iterationNorm;
        }
    }
    return norm;
}

std::vector<float> getRandomVector(int n) {
    std::random_device device;
    std::mt19937 gen(device());
    std::vector<float> vec(n);
    for (int i = 0; i < n; ++i) {
        vec[i] = gen() % 10 + 1.0f;
    }
    return vec;
}

std::vector<float> getRandomMatrix(int n) {
    std::vector<float> matrix(n * n);
    for (int i = 0; i < n; ++i) {
        std::vector<float> row = getRandomVector(n);
        matrix.insert(matrix.begin() + i * n, row.begin(), row.end());
    }
    return matrix;
}

std::vector<float> getX0(const std::vector<float> &A, const std::vector<float> &b, const int n) {
    std::vector<float> X0(n);
    for (int i = 0; i < n; ++i) {
        X0[i] = b[i] / A[i * n + i];
    }
    return X0;
}

std::vector<float> jacobiSequential(const std::vector<float> &A, const std::vector<float> &b,
                                    const std::vector<float> &X0, const float eps, const int n) {
    std::vector<float> x(X0);

    float norm = .0;

    do {
        std::vector<float> xk(b);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    xk[i] -= A[i * n + j] * x[j];
                }
            }
            xk[i] /= A[i * n + i];
        }

        norm = normDifferent(x, xk);
        x = xk;
    } while (norm >= eps);

    return x;
}

std::vector<float> jacobiParallel(const std::vector<float> &A, const std::vector<float> &b,
                                  const std::vector<float> &X0, const float eps, const int n) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int delta = n / size;

    std::vector<float> localA(n * n);
    if (rank == 0) {
        localA = A;
    }
    MPI_Bcast(localA.data(), n * n, MPI_FLOAT, 0, MPI_COMM_WORLD);
    std::vector<float> localB(n);
    if (rank == 0) {
        localB = b;
    }
    MPI_Bcast(localB.data(), n, MPI_FLOAT, 0, MPI_COMM_WORLD);
    std::vector<float> localX(n);
    if (rank == 0) {
        localX = X0;
    }
    MPI_Bcast(localX.data(), n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    float norm = .0;
    int start = delta * rank;
    int end = start + delta;

    do {
        std::vector<float> xk(b);
        for (int i = start; i < end; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    xk[i] -= localA[i * n + j] * localX[j];
                }
            }
            xk[i] /= localA[i * n + i];
        }

        std::vector<float> tmpXk(n);
        MPI_Allgather(xk.data() + delta * rank, delta, MPI_FLOAT, tmpXk.data(), delta, MPI_FLOAT, MPI_COMM_WORLD);
        xk = tmpXk;

        norm = normDifferent(localX, xk);
        localX = xk;
    } while (norm >= eps);

    return localX;
}
