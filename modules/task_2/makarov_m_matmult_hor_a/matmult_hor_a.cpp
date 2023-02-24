// Copyright 2023 Makarov Mikhail
#include "../../modules/task_2/makarov_m_matmult_hor_a/matmult_hor_a.h"

#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::vector<int> getRandVector(int vecSize) {
    std::random_device device;
    std::mt19937 generator(device());
    std::vector<int> vec(vecSize);
    for (int k = 0; k < vecSize; k++) vec[k] = generator() % 99;
    return vec;
}

void printAsMatrix(const std::vector<int>& vec, int m, int n) {
    for (int i = 0; i < m; i++) {
        std::cout << "( ";
        for (int j = 0; j < n - 1; j++) std::cout << vec[n * i + j] << ", ";
        std::cout << vec[n * i + (n - 1)];
        std::cout << " )\n";
    }
}

std::vector<int> multSequential(const std::vector<int>& matrixA,
                                const std::vector<int>& matrixB, int m1, int n1,
                                int m2, int n2) {
    int m3 = m1;
    int n3 = n2;
    std::vector<int> matrixRes(m3 * n3);

    // matrixRes elements calculation
    for (int i = 0; i < m3; i++) {
        for (int j = 0; j < n3; j++) {
            // matrixRes[i, j] = row i * col j
            int elem_ij = 0;

            for (int k = 0; k < n1; k++) {
                elem_ij += matrixA[i * n1 + k] * matrixB[j + k * n2];
            }

            matrixRes[i * n3 + j] = elem_ij;
        }
    }

    return matrixRes;
}

std::vector<int> multParallel(const std::vector<int>& matrixA,
                              const std::vector<int>& matrixB, int m1, int n1,
                              int m2, int n2) {
    int total_procs, wrank;
    MPI_Comm_size(MPI_COMM_WORLD, &total_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &wrank);

    const int useful_procs_cnt = std::min(total_procs, m1);
    const bool is_proc_useful = wrank < useful_procs_cnt;
    const bool is_proc_root = wrank == 0;

    // Defines custom communicator
    int color = (wrank < useful_procs_cnt) ? 1 : 2;
    MPI_Comm CUSTOM_COMM;
    MPI_Comm_split(MPI_COMM_WORLD, color, wrank, &CUSTOM_COMM);
    if (color != 1) {
        return std::vector<int>(0);
    }
    int custrank;
    MPI_Comm_rank(CUSTOM_COMM, &custrank);

    const int usual_rows_cnt = m1 / useful_procs_cnt;
    const int lost_rows_cnt = m1 % useful_procs_cnt;

    std::vector<int> matrixA_Part;

    std::vector<int> matrixB_Full(m2 * n2);
    if (is_proc_root) {
        matrixB_Full.assign(matrixB.begin(), matrixB.end());
    }

    // Broadcasting matrixB to all processes
    MPI_Bcast(matrixB_Full.data(), matrixB_Full.size(), MPI_INT, MY_ROOT, CUSTOM_COMM);

    if (is_proc_root) {
        matrixA_Part.assign(
            matrixA.data(),
            matrixA.data() + (usual_rows_cnt + lost_rows_cnt) * n1);

        // Sending usual_rows_cnt rows of matrix A to other processes
        for (size_t uproc_rank = 1; uproc_rank < useful_procs_cnt;
             uproc_rank++) {
            auto pStart =
                matrixA.data() + (usual_rows_cnt + lost_rows_cnt) * n1;
            auto pBuf = pStart + (uproc_rank - 1) * (usual_rows_cnt * n1);
            int msg_size = (usual_rows_cnt * n1);
            MPI_Send(pBuf, msg_size, MPI_INT, uproc_rank, 0, MPI_COMM_WORLD);
        }

    } else {
        // Receiving usual_rows_cnt rows of matrix A from the root
        int msg_size = usual_rows_cnt * n1;
        matrixA_Part.resize(msg_size);
        MPI_Status s;
        MPI_Recv(matrixA_Part.data(), msg_size, MPI_INT, MY_ROOT, 0,
                 MPI_COMM_WORLD, &s);

        /* {
            std::cout << "rank = " << wrank << '\n';
            std::cout << "Got matrix A part:\n";
            printAsMatrix(matrixA_Part, usual_rows_cnt, n1);
            std::cout << "\n"; 
            std::cout << "And matrixBFull:\n";
            printAsMatrix(matrixB_Full, m2, n2);
        } */
    }

    int ma_part = is_proc_root ? usual_rows_cnt + lost_rows_cnt : usual_rows_cnt;
    int na_part = n1;

    std::vector<int> matrixRes_part =
        multSequential(matrixA_Part, matrixB_Full, ma_part, na_part, m2, n2);

    if (is_proc_root) {
        int m3 = m1;
        int n3 = n2;
        std::vector<int> matrixRes(m3 * n3);

        for (size_t i = 0; i < matrixRes_part.size(); i++) {
            matrixRes[i] = matrixRes_part[i];
        }

        // Receiving results from other processes
        for (size_t uproc_rank = 1; uproc_rank < useful_procs_cnt;
             uproc_rank++) {
            auto pStart =
                matrixRes.data() + (usual_rows_cnt + lost_rows_cnt) * n2;
            auto pBuf = pStart + (uproc_rank - 1) * (n2 * usual_rows_cnt);
            int msg_size = usual_rows_cnt * n2;
            MPI_Status s;
            MPI_Recv(pBuf, msg_size, MPI_INT, uproc_rank, 0, MPI_COMM_WORLD, &s);
        }

        return matrixRes;
    } else {
        // Sending calc results to the root
        MPI_Send(matrixRes_part.data(), matrixRes_part.size(), MPI_INT, MY_ROOT,
                 0, MPI_COMM_WORLD);
    }

    return std::vector<int>(0);
}
