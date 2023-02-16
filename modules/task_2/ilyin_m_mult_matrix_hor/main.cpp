// Copyright 2023 Ilyin Maxim
#include <gtest/gtest.h>
#include <vector>
#include "./mult_matrix.h"
#include <gtest-mpi-listener.hpp>

TEST(Mult_Matrix_Hor_MPI, Test_Mult1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> A, B;
    const int n = 4, m = 4;

    if (rank == 0) {
        A = getRandomMatrix(n, m);
        B = getRandomMatrix(n, m);
    }
    std::vector<int> parallel_mult = getMultMatrixParallel(A, B, n, m);
    if (rank == 0) {
        std::vector<int> sequential_mult = getMultMatrixSequential(A, B, n, m);
        ASSERT_EQ(sequential_mult, parallel_mult);
    }
}

TEST(Mult_Matrix_Hor_MPI, Test_Mult2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> A, B;
    const int n = 10, m = 10;

    if (rank == 0) {
        A = getRandomMatrix(n, m);
        B = getRandomMatrix(n, m);
    }

    std::vector<int> parallel_mult = getMultMatrixParallel(A, B, n, m);
    if (rank == 0) {
        std::vector<int> sequential_mult = getMultMatrixSequential(A, B, n, m);
        ASSERT_EQ(sequential_mult, parallel_mult);
    }
}

TEST(Mult_Matrix_Hor_MPI, Test_Mult3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> A, B;
    const int n = 12, m = 12;

    if (rank == 0) {
        A = getRandomMatrix(n, m);
        B = getRandomMatrix(n, m);
    }

    std::vector<int> parallel_mult = getMultMatrixParallel(A, B, n, m);
    if (rank == 0) {
        std::vector<int> sequential_mult = getMultMatrixSequential(A, B, n, m);
        ASSERT_EQ(sequential_mult, parallel_mult);
    }
}

TEST(Mult_Matrix_Hor_MPI, Test_Mult4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> A, B;
    const int n = 20, m = 20;

    if (rank == 0) {
        A = getRandomMatrix(n, m);
        B = getRandomMatrix(n, m);
    }

    std::vector<int> parallel_mult = getMultMatrixParallel(A, B, n, m);
    if (rank == 0) {
        std::vector<int> sequential_mult = getMultMatrixSequential(A, B, n, m);
        ASSERT_EQ(sequential_mult, parallel_mult);
    }
}

TEST(Mult_Matrix_Hor_MPI, Test_Mult5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> A, B;
    const int n = 50, m = 50;

    if (rank == 0) {
        A = getRandomMatrix(n, m);
        B = getRandomMatrix(n, m);
    }

    std::vector<int> parallel_mult = getMultMatrixParallel(A, B, n, m);
    if (rank == 0) {
        std::vector<int> sequential_mult = getMultMatrixSequential(A, B, n, m);
        ASSERT_EQ(sequential_mult, parallel_mult);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
