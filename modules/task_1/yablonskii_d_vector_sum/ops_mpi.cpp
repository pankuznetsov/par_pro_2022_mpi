// Copyright 2023 Yablonskii Dmitriy
#include <mpi.h>
#include <math.h>
#include <iostream>
#include <random>
#include <algorithm>
#include "../../../modules/test_tasks/test_mpi/ops_mpi.h"

std::vector<int> rand_vec(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<int> r_vector(size);

    for (int i = 0; i < size; i++) {
        r_vector[i] = gen() % 10;
    }

    return r_vector;
}

int stand_summa(std::vector<int> V) {
    int size = V.size();


    int res = 0;

    for (int i = 0; i < size; i++) {
        res += V[i];
    }
    return res;
}

int par_summa(std::vector<int> Vector, int size) {
    int ProcRank, ProcNum;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int d = size / ProcNum;
    if (d == 0) d = 1;

    int t;
    if (ProcNum < size)
        t = ProcNum;
    else
        t = size;
    std::vector<int> cur_vec(d);
    if (ProcRank == 0) {
        cur_vec = std::vector<int>(Vector.begin(), Vector.begin() + d);
        for (size_t i = 1; i < t; i++) {
            MPI_Send(Vector.data() + i * d, d, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else if (ProcRank < size) {
        MPI_Status status;
        MPI_Recv(cur_vec.data(), d, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    if (ProcRank == 0) {
        if (size % t > 0) {
            size_t j = 0;
            cur_vec.resize(cur_vec.size() + size % ProcNum);
            for (int i = (size - size % ProcNum); i < size; i++) {
                cur_vec[j + d] = Vector[i];
                j++;
            }
        }
    }

    int fullsumma = 0;
    int cursumma = 0;
    if (ProcRank < size) cursumma = stand_summa(cur_vec);
    MPI_Reduce(&cursumma, &fullsumma, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    return fullsumma;
}
