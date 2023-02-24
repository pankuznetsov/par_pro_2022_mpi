// Copyright 2022 Nikiforova Lada
#include "../../../modules/task_2/nikiforova_l_lattice_torus/lattice_torus.h"

#include <mpi.h>

#include <cmath>
#include <iostream>
#include <vector>

struct Lattice_torus {
    int strLen;
    int upLen;
    int size;
    std::vector<int> descriptor;

    explicit Lattice_torus(int rank = 0, int size = 0) {
        this->size = size;
        this->strLen = ceil(sqrt(size));
        this->upLen = ceil(size / this->strLen);
        if (size % this->strLen != 0) this->upLen += 1;
        int xCoord = floor(rank / strLen);
        int yCoord = (rank % strLen);
        descriptor.push_back(xCoord);
        descriptor.push_back(yCoord);
    }
    int descToNom(std::vector<int> desc) {
        return (desc.at(0) * this->strLen) + desc.at(1);
    }
    std::vector<int> nomToDesc(int rank) {
        std::vector<int> res;
        int xCoord = floor(rank / this->strLen);
        int yCoord = (rank % this->strLen);
        res.push_back(xCoord);
        res.push_back(yCoord);
        return res;
    }
    int next(int proc) {
        std::vector<int> wayDesc = this->nomToDesc(proc);
        if (wayDesc == this->descriptor) return -1;
        if (wayDesc.at(1) != this->descriptor.at(1)) {
            int x = this->descriptor.at(1);
            int y = wayDesc.at(1);
            int len = this->strLen;
            if (std::abs(x - y) > len - std::abs(x - y)) {
                if (x > y)
                    x += 1;
                else
                    x -= 1;
                if (x < 0) x = len - 1;
                if (x == len) x = 0;
                return this->descToNom(
                    std::vector<int>{this->descriptor.at(0), x});
            } else {
                if (x > y)
                    x -= 1;
                else
                    x += 1;
                if (x < 0) x = len - 1;
                if (x == len) x = 0;
                return this->descToNom(
                    std::vector<int>{this->descriptor.at(0), x});
            }
        } else {
            int x = this->descriptor.at(0);
            int y = wayDesc.at(0);
            int len = this->upLen;
            if (std::abs(x - y) > len - std::abs(x - y)) {
                if (x > y)
                    x += 1;
                else
                    x -= 1;
                if (x < 0) x = len - 1;
                if (x == len) x = 0;
                return this->descToNom(
                    std::vector<int>{x, this->descriptor.at(1)});
            } else {
                if (x > y)
                    x -= 1;
                else
                    x += 1;
                if (x < 0) x = len - 1;
                if (x == len) x = 0;
                return this->descToNom(
                    std::vector<int>{x, this->descriptor.at(1)});
            }
        }
    }
};

int optimalRoute(int msg, int sender, int host) {
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    Lattice_torus lat(rank, size);
    if (rank == sender) {
        int message = msg;
        int next = lat.next(host);
        if (next == -1) {
            for (int i = 0; i < size; i++) {
                if (i == rank) continue;
                int ExitFlag = -1;
                MPI_Send(&ExitFlag, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            return message;
        }
        MPI_Send(&message, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }
    int messageBuf;
    while (true) {
        MPI_Status status;
        MPI_Recv(&messageBuf, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
                 &status);
        if (messageBuf == -1)
            return 0;
        else if (host == rank)
            break;
        int next = lat.next(host);
        MPI_Send(&messageBuf, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }
    for (int i = 0; i < size; i++) {
        if (i == rank) continue;
        int ExitFlag = -1;
        MPI_Send(&ExitFlag, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
    return messageBuf;
}
