// Copyright 2022 Polyackov Lev
#include "../../../modules/task_2/polyackov_l_star_topology/star_topology.h"

void MPI_Send_With_Star(void* message, int count, MPI_Datatype datatype, int from,
                        int dest, int tag, MPI_Comm comm, std::vector<int>* path) {
    (*path).clear();
    if (from == dest) {
        (*path).push_back(from);
        (*path).push_back(dest);
        return;
    }

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank != from) {
        if (rank == 0 && dest != 0) {
            MPI_Status status;
            MPI_Datatype* buf = new MPI_Datatype[count];

            MPI_Recv(buf, count, datatype, from, tag, comm, &status);
            (*path).push_back(from);
            MPI_Send(buf, count, datatype, dest, tag, comm);
            (*path).push_back(dest);
            delete[] buf;
            (*path).push_back(rank);
        }
        return;
    } else {
        if (from != 0 && dest != 0) {
            (*path).push_back(rank);
            MPI_Send(message, count, datatype, 0, tag, comm);
        } else {
            (*path).push_back(rank);
            MPI_Send(message, count, datatype, dest, tag, comm);
        }
        return;
    }
}

void MPI_Star_create(MPI_Comm* comm) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::vector<int> index;
    index.resize(size);
    int it = 0;
    for (int& tmp : index) {
        tmp = size - 1 + it;
        it++;
    }

    std::vector<int> edges;
    edges.resize(2 * (size - 1));
    std::fill(edges.begin(), edges.end(), 0);
    for (int i = 0; i < size - 1; i++) {
        edges[i] = i + 1;
    }
    if (size == 1) {
        edges.push_back(0);
    }

    MPI_Graph_create(MPI_COMM_WORLD, size, index.data(), edges.data(), false, comm);
}
