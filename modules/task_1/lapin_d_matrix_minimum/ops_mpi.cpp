// Copyright 2022 Lapin Dmitriy
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <iostream>
#include "./ops_mpi.h"

int getSequentialOperations(std::vector<std::vector<int>> matrix) {
    int min = matrix[0][0];
    for (int i = 0; i < matrix.size(); i++)
        for (int j = 0; j < matrix[i].size(); j++)
            if (matrix[i][j] < min) {
                min = matrix[i][j];
            }
    return min;
}

int getParallelOperations(std::vector<std::vector<int>> matrix) {
    int size, rank;
    int result = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int old_size = 0;
        if (matrix[0].size() < size) {
            old_size = size;
            size = matrix[0].size();
        }
        for (int i = 1; i < size; i++) {
            int not_run = 0;
            MPI_Send(&not_run, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        for (int i = size; i < old_size; i++) {
            int not_run = 1;
            MPI_Send(&not_run, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        int a = matrix[0].size() / size;
        int b = matrix[0].size() % size;
        std::vector<int> matrix_lenta;
        matrix_lenta = getVectorTransponMt(matrix);
        for (int i = 1; i < size; i++) {
            int len = matrix.size() * a;
            MPI_Send(&len, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(matrix_lenta.data() + (b + a*i) * matrix.size(), matrix.size() * a, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        int min = matrix[0][0];
        for (int j = 0; j < a + b; j++) {
            for (int i = 0; i < matrix.size(); i++) {
                if (matrix[i][j] < min) {
                    min = matrix[i][j];
                }
            }
        }
        MPI_Status status;
        for (int i = 1; i < size; i++)
        {
            int temp;
            MPI_Recv(&temp, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (temp < min) {
                min = temp;
            }
        }
        result = min;
    }
    else {
        MPI_Status status;
        int not_run;
        MPI_Recv(&not_run, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        if (not_run == 1) {
            return 0;
        }
        int len;
        MPI_Recv(&len, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        std::vector<int> local_vec(len);
        MPI_Recv(local_vec.data(), len, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        int min = local_vec[0];
        for (int i = 0; i < len; i++) {
            if (local_vec[i] < min)
                min = local_vec[i];
        }
        MPI_Send(&min, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    return result;
}

std::vector<std::vector<int>> CreateMatrix(int m, int n) {
    std::vector<std::vector<int>> matrix(m);
    srand(100);
    for (int i = 0; i < m; i++) {
        matrix[i].resize(n);
    }
    std::mt19937 gen(1);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = gen() % 10000;
    return matrix;

}

std::vector <int> getVectorTransponMt(const std::vector<std::vector<int>>& matrix) {
    std::vector<int> tmp(matrix.size()* matrix[0].size());
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            tmp[i + j * matrix.size()] = matrix[i][j];
        }
    }
    return tmp;
}