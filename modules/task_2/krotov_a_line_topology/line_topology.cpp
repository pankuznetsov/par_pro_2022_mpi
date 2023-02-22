// Copyright 2023 Krotov Aleksey
#include "../../../modules/task_2/krotov_a_line_topology/line_topology.h"
#include <mpi.h>
#include <exception>
#include <iostream>
#include <vector>

void vecPrint(const std::vector<int>& targetVec) {
    for (size_t i = 0; i < targetVec.size() - 1; i++)
        std::cout << targetVec[i] << " ";
    std::cout << targetVec[targetVec.size() - 1] << '\n';
}

void LineSend(void* buf, int count, MPI_Datatype datatype, int from, int dest,
              int tag, MPI_Comm comm) {
    int comm_size, cur_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &cur_rank);

    if (from == dest) {
        throw std::runtime_error(
            "Unable to perform LineSend operation: Don't use LineSend when "
            "from is dest.");
    }
    if (comm_size < 2) {
        throw std::runtime_error(
            "Unable to perform LineSend operation: Current communicator has "
            "too small size.");
    }
    if (from >= comm_size || dest >= comm_size) {
        throw std::runtime_error(
            "Unable to perform LineSend operation: too large ranks of "
            "processes.");
    }

    const int distance = std::abs(from - dest);

    if (distance == 1) {
        // If processes are neighbours, send works directly
        if (cur_rank == from) {
            MPI_Send(buf, count, datatype, dest, tag, comm);
        } else if (cur_rank == dest) {
            MPI_Status stat;
            MPI_Recv(buf, count, datatype, from, tag, comm, &stat);
        }
    } else {
        // In other case, code makes series of sends/recvs in some direction
        if (from < dest) {
            // Sending to the right
            for (int r = from; r <= dest - 1; r++) {
                if (cur_rank == r) {
                    MPI_Send(buf, count, datatype, r + 1, tag, comm);
                } else if (cur_rank == (r + 1)) {
                    MPI_Status stat;
                    MPI_Recv(buf, count, datatype, r, tag, comm, &stat);
                }
            }
        } else {
            // Sending to the left
            for (int r = from; r >= dest + 1; r--) {
                if (cur_rank == r) {
                    MPI_Send(buf, count, datatype, r - 1, tag, comm);
                } else if (cur_rank == (r - 1)) {
                    MPI_Status stat;
                    MPI_Recv(buf, count, datatype, r, tag, comm, &stat);
                }
            }
        }
    }
}
