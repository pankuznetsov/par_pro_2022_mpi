// Copyright 2023 Ilyin Maxim
#include <mpi.h>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <algorithm>
#include "../../../modules/task_2/ilyin_m_mult_matrix_hor/mult_matrix.h"

std::vector<int> getRandomMatrix(int n, int m) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> matrix(n*m);

    for (int i = 0; i < n * m; i++) {
        matrix[i] = static_cast<int>(gen() % 100);
    }
    return matrix;
}

std::vector<int> transposeMatrix(const std::vector<int> &matrix, int n, int m) {
    const int size = matrix.size();
    std::vector<int> vec(size);
    int k = 0;
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            vec[k++] += matrix[i * m + j];
        }
    }
    return vec;
}

std::vector<int> getMultMatrixSequential(const std::vector<int>& pMatrix1,
                            const std::vector<int>& pMatrix2, int n, int m) {
    std::vector<int> pResult(n * m);
    std::vector<int> A(n * m), B(n * m);
    A = pMatrix1;
    B = transposeMatrix(pMatrix2, n, m);
    int k = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int t = 0; t < m; t++) {
                pResult[k] += A[i * n + t] * B[m * j + t];
            }
            k++;
        }
    }
    return pResult;
}

std::vector<int> getMultMatrixSequential2(const std::vector<int>& pMatrix1,
                            const std::vector<int>& pMatrix2, int n, int m, int start, int finish) {
    std::vector<int> pResult((finish - start) * m);
    std::vector<int> A, B;
    A = pMatrix1;
    B = transposeMatrix(pMatrix2, n, m);
    const int size_matrix = pMatrix1.size();
    int k = 0;
    for (int i = 0; i < size_matrix / n; i++) {
        for (int j = 0; j < m; j++) {
            for (int t = 0; t < m; t++) {
                pResult[k] += A[i * n + t] * B[m * j + t];
            }
            k++;
        }
    }
    return pResult;
}

void printMatrix(std::vector<int> vec) {
    const int size = vec.size();
    for (int i = 0; i < size; i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

std::vector<int> getMultMatrixParallel(const std::vector<int>& A, const std::vector<int>& B, int n, int m) {
    std::vector<int> global_vec(m*n);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int delta = n/size;
    int* constant = new int[size];
    int* displs = new int[size];
    int* recvcounts = new int[size];
    int* start = new int[size];
    int* finish = new int[size];
    if (size >= n) {
        for (int i = 0; i < size; i++) {
            constant[i] = m;
            recvcounts[i] = 1 * m;
            displs[i] = m * i;
            start[i] = i;
            finish[i] = i + 1;
        }
    } else {
        int tmp = 0;
        for (int i = 0; i < size; i++) {
        if (i != size - 1) {
            constant[i] = delta * m;
            tmp += constant[i];
            recvcounts[i] = constant[i];
            displs[i] = i * recvcounts[i];
            start[i] = i * delta;
            finish[i] = (i + 1) * delta;
        } else {
            constant[i] = n * m - tmp;
            recvcounts[i] = constant[i];
            displs[i] = i * recvcounts[i-1];
            start[i] = 0;
            finish[i] = n;
            }
        }
    }

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            MPI_Send(A.data() + proc * constant[proc-1], constant[proc],
                        MPI_INT, proc, 0, MPI_COMM_WORLD);
            MPI_Send(B.data(), m*n, MPI_INT, proc, 1, MPI_COMM_WORLD);
            MPI_Send(&constant[proc], 1, MPI_INT, proc, 2, MPI_COMM_WORLD);
        }
    }
    int local_size_vec = 0;
    if (rank != 0) {
        MPI_Status status1;
        MPI_Recv(&local_size_vec, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status1);
    }
    std::vector<int> local_row(local_size_vec);
    std::vector<int> local_vec(m * n);
    if (rank == 0) {
        local_row = std::vector<int>(A.begin(), A.begin() + constant[0]);
        local_vec = std::vector<int>(B.begin(), B.begin() + m * n);
    } else {
        MPI_Status status;
        MPI_Recv(local_row.data(), local_size_vec, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(local_vec.data(), m * n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    }
    std::vector<int> local_value = getMultMatrixSequential2(local_row, local_vec, n, m, start[rank], finish[rank]);
    std::vector<int> test = std::vector<int>(local_value.begin(), local_value.begin() + recvcounts[rank]);
    // local_value = std::vector<int>(local_value.begin(), local_value.begin() + recvcounts[rank]);
    MPI_Gatherv(test.data(), test.size(), MPI_INT, global_vec.data(), recvcounts, displs,
                                        MPI_INT, 0, MPI_COMM_WORLD);
    return global_vec;
}
