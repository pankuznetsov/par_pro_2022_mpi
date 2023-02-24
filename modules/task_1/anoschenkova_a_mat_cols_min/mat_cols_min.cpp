// Copyright 2023 Anoschenkova Anna
#include "../../../modules/task_1/anoschenkova_a_mat_cols_min/mat_cols_min.h"

#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::vector<int> randColMat(int m, int n) {
    std::random_device dev;
    std::mt19937 gen(dev());
    int sz = m * n;

    std::vector<int> mat(sz);

    int i = 0;
    while (i < sz) mat[i] = gen() % 5000, i++;

    return mat;
}

void vecShow(const std::vector<int>& vec) {
    std::cout << std::endl;
    for (size_t i = 0; i < vec.size(); i++) std::cout << vec[i] << " ";
    std::cout << std::endl;
}

std::vector<int> minByColsSequential(int m, int n,
                                     const std::vector<int>& mat) {
    std::vector<int> resVec(n);

    // Going through all columns
    for (int j = 0; j < n; j++) {
        // Recording min of every column
        int min = mat[j * m];
        for (int i = 0; i < m; i++) {
            if (mat[j * m + i] < min) min = mat[j * m + i];
        }
        resVec[j] = min;
    }

    return resVec;
}

std::vector<int> minByColsParallel(int m, int n, const std::vector<int>& mat) {
    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // We will work only with processes processes
    const int workers_count = std::min(n, comm_size);
    const bool isRoot = rank == 0;
    const bool isWorker = (rank != 0) && (rank < workers_count);
    // Every working process computes tasks tasks except rank = 0
    // (it computes (task + other) tasks)
    const int tasks_per_worker = n / workers_count;
    const int tasks_remained = n % workers_count;

    // Sending data to other processes
    if (isRoot) {
        for (int wrank = 1; wrank < workers_count; wrank++)
            MPI_Send(mat.data() + m * (tasks_per_worker + tasks_remained) +
                         (wrank - 1) * m * tasks_per_worker,
                     m * tasks_per_worker, MPI_INT, wrank, 0, MPI_COMM_WORLD);
    }

    std::vector<int> receivedMatPart(tasks_per_worker * m);
    std::vector<int> locRes;
    std::vector<int> globRes(n);

    if (isRoot) {
        receivedMatPart = std::vector<int>(
            mat.begin(), mat.begin() + (tasks_per_worker + tasks_remained) * m);

        locRes = minByColsSequential(m, (tasks_per_worker + tasks_remained),
                                     receivedMatPart);

        for (size_t j = 0; j < locRes.size(); j++) {
            globRes[j] = locRes[j];
        }

        for (size_t j = 1; j < workers_count; j++) {
            MPI_Status status;
            MPI_Recv(globRes.data() + tasks_remained + tasks_per_worker * j,
                     tasks_per_worker, MPI_INT, j, 0, MPI_COMM_WORLD, &status);
        }

    } else if (isWorker) {
        MPI_Status status;
        MPI_Recv(receivedMatPart.data(), tasks_per_worker * m, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, &status);

        locRes = minByColsSequential(m, tasks_per_worker, receivedMatPart);

        MPI_Send(locRes.data(), tasks_per_worker, MPI_INT, 0, 0,
                 MPI_COMM_WORLD);
    }

    return globRes;
}
