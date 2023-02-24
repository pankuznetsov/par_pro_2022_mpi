// Copyright 2022 Davydov Andrey
#include <gtest/gtest.h>
#include <vector>
#include "./jacobi.h"
#include <gtest-mpi-listener.hpp>

TEST(JACOBI, N_4_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int n = 4;
    const float eps = .01;
    std::vector<float> A = {
        6.0f, 2.0f, 2.0f, 1.0f,
        2.0f, 7.0f, 3.0f, 1.0f,
        3.0f, 2.0f, 7.0f, 1.0f,
        3.0f, 3.0f, 7.0f, 14.0f
    };
    std::vector<float> b = { 11.0f, 13.0f, 13.0f, 27.0f };
    std::vector<float> X0 = getX0(A, b, n);

    std::vector<float> parallelResult = jacobiParallel(A, b, X0, eps, n);

    if (rank == 0) {
        std::vector<float> sequentialResult = jacobiSequential(A, b, X0, eps, n);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

TEST(JACOBI, N_4_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int n = 4;
    const float eps = .01;
    std::vector<float> A = {
        20.9f, 1.2f, 2.1f, 0.9f,
        1.2f, 21.2f, 1.5f, 2.5f,
        2.1f, 1.5f, 19.8f, 1.3f,
        0.9f, 2.5f, 1.3f, 32.1f
    };
    std::vector<float> b = { 21.7f, 27.46f, 28.76f, 49.72f };
    std::vector<float> X0 = getX0(A, b, n);

    std::vector<float> parallelResult = jacobiParallel(A, b, X0, eps, n);

    if (rank == 0) {
        std::vector<float> sequentialResult = jacobiSequential(A, b, X0, eps, n);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

TEST(JACOBI, N_4_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int n = 4;
    const float eps = .01;
    std::vector<float> A = {
        3.0f, 0.0f, 0.0f, -3.0f,
        0.0f, 11.0f, 1.0f, 2.0f,
        1.0f, 0.0f, 5.0f, 0.0f,
        -1.0f, 2.0f, 7.0f, 11.0f
    };
    std::vector<float> b = { 21.0f, 21.0f, 48.0f, 61.0f };
    std::vector<float> X0 = getX0(A, b, n);

    std::vector<float> parallelResult = jacobiParallel(A, b, X0, eps, n);

    if (rank == 0) {
        std::vector<float> sequentialResult = jacobiSequential(A, b, X0, eps, n);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

TEST(JACOBI, N_4_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int n = 4;
    const float eps = .01;
    std::vector<float> A = {
        3.0f, 0.0f, 0.0f, -3.0f,
        0.0f, 11.0f, 1.0f, 2.0f,
        1.0f, 0.0f, 5.0f, 0.0f,
        -1.0f, 2.0f, 7.0f, 11.0f
    };
    std::vector<float> b = { 21.0f, 21.0f, 48.0f, 61.0f };
    std::vector<float> X0 = getX0(A, b, n);

    std::vector<float> parallelResult = jacobiParallel(A, b, X0, eps, n);

    if (rank == 0) {
        std::vector<float> sequentialResult = jacobiSequential(A, b, X0, eps, n);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

TEST(JACOBI, N_4_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int n = 4;
    const float eps = .01;
    std::vector<float> A = {
        3.0f, 0.0f, 0.0f, -3.0f,
        0.0f, 11.0f, 1.0f, 2.0f,
        1.0f, 0.0f, 5.0f, 0.0f,
        -1.0f, 2.0f, 7.0f, 11.0f
    };
    std::vector<float> b = { 21.0f, 21.0f, 48.0f, 61.0f };
    std::vector<float> X0 = getX0(A, b, n);

    std::vector<float> parallelResult = jacobiParallel(A, b, X0, eps, n);

    if (rank == 0) {
        std::vector<float> sequentialResult = jacobiSequential(A, b, X0, eps, n);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
            ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
