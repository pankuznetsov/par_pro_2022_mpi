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


    if (matrix[0].size() < size) {
        size = matrix[0].size();
        if (rank >= matrix[0].size()) {
            return 0;
        }
    }



    int a = matrix[0].size() / size;
    int b = matrix[0].size() % size;
    if (rank == 0) {
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

        int min = matrix[0][a + b + (rank - 1) * a];

        for (int j = a + b + (rank - 1) * a; j < a + b + rank * a; j++) {
            for (int i = 0; i < matrix.size(); i++) {
                if (matrix[i][j] < min)
                    min = matrix[i][j];
            }
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

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = rand()%10000;

    return matrix;

}