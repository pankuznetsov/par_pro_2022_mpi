// Copyright 2022 Tuzhilkina Polina
#include "../../../modules/task_2/tuzhilkina_p_star_top/star_top.h"

#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <functional>

// Root process is the center of our star
void StarSend(void* buf, int count, MPI_Datatype datatype, int from, int dest,
              int tag, MPI_Comm comm) {
    if (from == dest) return;

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (from == 0 || dest == 0) {
        // If "from" or "dest" is root, we send directly from "from" to "dest"
        if (rank == from) {
            MPI_Send(buf, count, datatype, dest, tag, comm);
        } else if (rank == dest) {
            MPI_Status stat;
            MPI_Recv(buf, count, datatype, from, tag, comm, &stat);
        }
    } else {
        // If not, we send data from source (rank != 0) to root, and then from root to dest
        if (rank == from) {
            MPI_Send(buf, count, datatype, 0, tag, comm);
        } else if (rank == 0) {
            MPI_Status stat;
            MPI_Recv(buf, count, datatype, from, tag, comm, &stat);

            MPI_Send(buf, count, datatype, dest, tag, comm);
        } else if (rank == dest) {
            MPI_Status stat;
            MPI_Recv(buf, count, datatype, 0, tag, comm, &stat);
        }
    }
}

void printVecElements(const std::vector<int>& vec) {
    for (size_t i = 0; i < vec.size() - 1; i++) {
        std::cout << vec[i] << ", ";
    }
    std::cout << vec[vec.size() - 1];
}
