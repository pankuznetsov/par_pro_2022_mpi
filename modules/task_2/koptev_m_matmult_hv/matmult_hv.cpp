// Copyright 2023 Koptev Maksim
#include "../../../modules/task_2/koptev_m_matmult_hv/matmult_hv.h"

#include <mpi.h>

#include <algorithm>
#include <ctime>
#include <numeric>
#include <random>
#include <vector>

std::vector<int> getRandomMatrix(int matSize) {
    if (matSize < 1) throw "Wrong length";
    const int size_matrix = matSize * matSize;
    std::vector<int> matrix(size_matrix);
    std::mt19937 gen{std::random_device()()};
    std::uniform_int_distribution<int> uid(-9, 9);
    std::generate(matrix.begin(), matrix.end(),
                  [&uid, &gen]() -> int { return uid(gen); });
    return matrix;
}

std::vector<int> getMatrixMultSeq(std::vector<int> matA,
                                         std::vector<int> matB,
                                         int matSize) {
    std::vector<int> matrix_c(matSize * matSize);
    for (int i = 0; i < matSize; i++) {
        for (int j = 0; j < matSize; j++) {
            matrix_c[i * matSize + j] = 0;
            for (int k = 0; k < matSize; k++) {
                matrix_c[i * matSize + j] =
                    matrix_c[i * matSize + j] +
                    matA[i * matSize + k] *
                        matB[k * matSize + j];
            }
        }
    }
    return matrix_c;
}

std::vector<int> getMatrixMultParellel(std::vector<int> matA,
                                                 std::vector<int> matB,
                                                 int matSize) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int lengthSmallBlock = matSize / size;
    const int lengthBigBlock = matSize / size + 1;
    const int residue_size = matSize % size;
    int lenghtCurrBlock;

    std::vector<int> resulting_matrix(matSize * matSize);
    std::vector<int> localVeca, localVecb, local_vec_b1, localVecc;

    if (matSize / size < 1) {
        if (rank == 0) {
            resulting_matrix =
                getMatrixMultSeq(matA, matB, matSize);
        }
        return resulting_matrix;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Datatype MPI_SMALL_BLOCK, MPI_BIG_BLOCK;

    MPI_Type_vector(matSize, lengthSmallBlock, matSize,
                    MPI_INT, &MPI_SMALL_BLOCK);
    MPI_Type_commit(&MPI_SMALL_BLOCK);
    MPI_Type_vector(matSize, lengthBigBlock, matSize,
                    MPI_INT, &MPI_BIG_BLOCK);
    MPI_Type_commit(&MPI_BIG_BLOCK);

    if (rank < residue_size) {
        localVeca.resize(matSize * lengthBigBlock);
        localVecc.resize(matSize * lengthBigBlock);
    } else {
        localVeca.resize(matSize * lengthSmallBlock);
        localVecc.resize(matSize * lengthSmallBlock);
    }

    if (residue_size == 0) {
        localVecb.resize(matSize * lengthSmallBlock);
        if (rank == 0) {
            local_vec_b1.resize(matSize * lengthSmallBlock);
        }
    } else {
        localVecb.resize(matSize * lengthBigBlock);
        if (rank == 0) {
            local_vec_b1.resize(matSize * lengthBigBlock);
        }
    }

    int* sendcounts = new int[size];
    int* displs = new int[size];

    for (int i = 0; i < size; i++) {
        displs[i] = 0;
        if (i < residue_size) {
            sendcounts[i] = lengthBigBlock * matSize;
        } else {
            sendcounts[i] = lengthSmallBlock * matSize;
        }
        if (i > 0) {
            displs[i] = displs[i - 1] + sendcounts[i - 1];
        }
    }

    if (rank < residue_size)
        lenghtCurrBlock = lengthBigBlock;
    else
        lenghtCurrBlock = lengthSmallBlock;

    MPI_Scatterv(matA.data(), sendcounts, displs, MPI_INT,
                 &localVeca.front(), sendcounts[rank], MPI_INT, 0,
                 MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < matSize; i++) {
            for (int j = 0; j < lenghtCurrBlock; j++) {
                localVecb[i * lenghtCurrBlock + j] =
                    matB[j + matSize * i];
            }
        }

        for (int i = 1; i < residue_size; i++)
            MPI_Send(matB.data() + i * lengthBigBlock, 1, MPI_BIG_BLOCK,
                     i, 0, MPI_COMM_WORLD);

        int f = residue_size;
        if (residue_size == 0)
            f = 1;

        for (int i = f; i < size; i++) {
            MPI_Send(matB.data() + residue_size * lengthBigBlock +
                         (i - residue_size) * lengthSmallBlock,
                     1, MPI_SMALL_BLOCK, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status status;
        if (rank < matSize % size)
            MPI_Recv(&localVecb.front(), matSize * lengthBigBlock,
                     MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        else
            MPI_Recv(&localVecb.front(),
                     matSize * lengthSmallBlock, MPI_INT, 0, 0,
                     MPI_COMM_WORLD, &status);
    }

    int size_recv_block;
    int indent_matrix = 0;

    for (int i = 0; i < rank; i++) {
        if (i % size < residue_size)
            lenghtCurrBlock = lengthBigBlock;
        else
            lenghtCurrBlock = lengthSmallBlock;

        indent_matrix += lenghtCurrBlock;
    }

    for (int i = 0; i < size; i++) {
        if ((rank + i) % size < residue_size)
            lenghtCurrBlock = lengthBigBlock;
        else
            lenghtCurrBlock = lengthSmallBlock;

        for (size_t j = 0; j < localVeca.size() / matSize; j++) {
            for (int l = 0; l < matSize; l++) {
                for (int k = 0; k < lenghtCurrBlock; k++) {
                    localVecc[(indent_matrix + j * matSize + k) %
                                localVecc.size()] =
                        localVecc[(indent_matrix + j * matSize + k) %
                                    localVecc.size()] +
                        localVeca[matSize * j + l] *
                            localVecb[k + l * lenghtCurrBlock];
                }
            }
        }

        indent_matrix += lenghtCurrBlock;

        if (indent_matrix == matSize)
            indent_matrix = 0;

        if (i < size - 1) {
            MPI_Status status;
            MPI_Send(&lenghtCurrBlock, 1, MPI_INT, (rank + size - 1) % size,
                     0, MPI_COMM_WORLD);
            MPI_Recv(&size_recv_block, 1, MPI_INT, (rank + 1) % size, 0,
                     MPI_COMM_WORLD, &status);

            if (rank == 0) {
                local_vec_b1 = localVecb;
                MPI_Recv(&localVecb.front(),
                         matSize * size_recv_block, MPI_INT,
                         (rank + 1) % size, 1, MPI_COMM_WORLD, &status);
                MPI_Send(local_vec_b1.data(),
                         matSize * lenghtCurrBlock, MPI_INT,
                         (rank + size - 1) % size, 1, MPI_COMM_WORLD);
            } else {
                MPI_Send(localVecb.data(),
                         matSize * lenghtCurrBlock, MPI_INT,
                         (rank + size - 1) % size, 1, MPI_COMM_WORLD);
                MPI_Recv(&localVecb.front(),
                         matSize * size_recv_block, MPI_INT,
                         (rank + 1) % size, 1, MPI_COMM_WORLD, &status);
            }
        }
    }

    MPI_Gatherv(localVecc.data(), sendcounts[rank], MPI_INT,
                &resulting_matrix.front(), sendcounts, displs, MPI_INT, 0,
                MPI_COMM_WORLD);
    MPI_Type_free(&MPI_BIG_BLOCK);
    MPI_Type_free(&MPI_SMALL_BLOCK);
    return resulting_matrix;
}
