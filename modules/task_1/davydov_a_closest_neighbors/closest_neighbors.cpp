// Copyright 2022 Davydov Andrey
#include <mpi.h>
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/davydov_a_closest_neighbors/closest_neighbors.h"

std::vector<int> getRandomVector(int size) {
    std::random_device device;
    std::mt19937 gen(device());
    std::vector<int> vec(size);
    for (int i = 0; i < size; ++i) {
        vec[i] = gen() % 10;
    }
    return vec;
}

int closestNeighborsSequential(std::vector<int> vec) {
    int index = 0;
    int diff = std::abs(vec[0] - vec[1]);

    const int vecSize = static_cast<int>(vec.size());

    for (int i = 1; i < vecSize - 1; ++i) {
        int iterationDiff = std::abs(vec[i] - vec[i + 1]);
        if (iterationDiff < diff) {
            diff = iterationDiff;
            index = i;
        }
    }

    return index;
}

int closestNeighborsParallel(std::vector<int> vec, const int vecSize) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int delta = vecSize / size;

    if (rank == 0) {
        for (int proc = 1; proc < size; ++proc) {
            MPI_Send(vec.data() + delta * proc, delta,
                        MPI_INT, proc, 0, MPI_COMM_WORLD);
        }
    }

    std::vector<int> local(delta);

    if (rank == 0) {
        local = std::vector<int>(vec.begin(), vec.begin() + delta);
    } else {
        MPI_Status status;
        MPI_Recv(local.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    int localIndex = closestNeighborsSequential(local);

    std::vector<int> results(size);

    MPI_Gather(&localIndex, 1, MPI_INT, results.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        int index = results[0];
        int diff = std::abs(vec[index] - vec[index + 1]);
        for (int i = 1; i < size; ++i) {
            int newIndex = results[i];
            int iterationDiff = std::abs(vec[newIndex] - vec[newIndex + 1]);
            if (iterationDiff < diff) {
                diff = iterationDiff;
                index = newIndex;
            }
        }

        return index;
    }

    return 0;
}
