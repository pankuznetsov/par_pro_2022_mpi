// Copyright 2022 Tsvetkov
#include <mpi.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <random>
#include <iostream>
#include "../../../modules/task_1/tsvetkov_a_vec_sum/vector_sum.h"

std::vector<int> getRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = gen() % 10;
    }
    return vec;
}

void printVector(const std::vector<int>& vec) {
    for (size_t i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << ", ";
    }
}

int getSumSequential(std::vector<int> vec) {
    int result = 0;
    for (size_t i = 0; i < vec.size(); i++) result += vec[i];
    return result;
}

int getSumParallel(std::vector<int> globalVector) {
    int ProcNum, ProcRank, n;
    int sum = 0, sumall = 0;
    n = globalVector.size();
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    if (ProcRank == 0) {
        while (n % ProcNum != 0) {
            globalVector.push_back(0);
            n++;
        }
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    n = n / ProcNum;
    std::vector<int> local(n);
    MPI_Scatter(globalVector.data(), n, MPI_INT, local.data(), n, MPI_INT, 0,
                MPI_COMM_WORLD);
    // for (int i = 0; i < n; i++)
    // printf("process num %d ---- %d \n", ProcRank, local[i]);
    for (int i = 0; i < n; i++) sum += local[i];
    MPI_Reduce(&sum, &sumall, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (ProcRank == 0) {
        printf("\nresult %d ", sumall);
        return sumall;
    }
    return 0;
}
