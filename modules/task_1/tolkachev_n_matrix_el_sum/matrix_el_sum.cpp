// Copyright 2022 Tolkachev Nikita
#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/tolkachev_n_matrix_el_sum/matrix_el_sum.h"
std::vector<int> getRandomMatrix(int m, int n) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> matrix(m * n);
    for (int i = 0; i < m * n; i++) {
        matrix[i] = gen() % 1000;
    }
    return matrix;
}
int getVectorSum(std::vector<int> vec) {
    int res = 0;
    for (int i = 0; i < vec.size(); i++) {
        res += vec[i];
    }
    return res;
}
int getParallelSum(std::vector<int> global_matrix) {
    int comm_size, rank;
    int global_sum = 0;
    int size_matrix = global_matrix.size();
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int loc_size = size_matrix / comm_size;
    const int remainder = size_matrix % comm_size;
    if (loc_size == 0) {
        if (rank == 0) {
            std::vector<int> local_vec(remainder);
            local_vec = std::vector<int>(global_matrix.begin(),
                global_matrix.begin() + remainder);
            int local_sum = getVectorSum(local_vec);
            global_sum = local_sum;
        }
    } else {
        std::vector<int> local_vec(loc_size);
        if (rank == 0) {
            for (int numProc = 1; numProc < comm_size; numProc++) {
                MPI_Send(global_matrix.data() + remainder + numProc * loc_size,
                    loc_size, MPI_INT, numProc, 0, MPI_COMM_WORLD);
            }
            local_vec.resize(loc_size + remainder);
            local_vec = std::vector<int>(global_matrix.begin(),
                global_matrix.begin() + loc_size + remainder);
        } else {
            MPI_Status status;
            MPI_Recv(local_vec.data(), loc_size, MPI_INT, 0, 0, MPI_COMM_WORLD,
                &status);
        }
        int local_sum = getVectorSum(local_vec);
        MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0,
            MPI_COMM_WORLD);
     }
    return global_sum;
}
