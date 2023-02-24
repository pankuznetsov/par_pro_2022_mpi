// Copyright 2022 Dryamina Kseniya
#include <mpi.h>
#include <algorithm>
#include <vector>
#include <random>
#include "../../../modules/task_1/dryamina_k_vector_min/vector_min.h"

std::vector<int> getRandomVector(int size) {
    std::random_device device;
    std::mt19937 gen(device());
    std::vector<int> vec(size);
    for (int i = 0; i < size; ++i) {
        vec[i] = gen() % 10;
    }
    return vec;
}

int vectorMinSequential(std::vector<int> vec) {
    const int size = vec.size();
    int reduction = vec[0];
    for (int i = 1; i < size; ++i) {
        reduction = std::min(reduction, vec[i]);
    }
    return reduction;
}

int vectorMinParallel(std::vector<int> vec, const int vecSize) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int delta = vecSize / size;
    const int diff = vecSize % size;

    if (rank == 0) {
        for (int proc = 1; proc < size; ++proc) {
            int offset = proc * delta;
            int count = delta + ((proc + 1) / size) * diff;
            MPI_Send(vec.data() + offset, count,
                        MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> local(rank == size - 1 ? delta + diff : delta);
    if (rank == 0) {
        local = std::vector<int>(vec.begin(), vec.begin() + delta);
    } else {
        int count = delta + ((rank + 1) / size) * diff;
        MPI_Status status;
        MPI_Recv(local.data(), count, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int reduction = 0;
    int localReduction = vectorMinSequential(local);

    MPI_Reduce(&localReduction, &reduction, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    return reduction;
}
