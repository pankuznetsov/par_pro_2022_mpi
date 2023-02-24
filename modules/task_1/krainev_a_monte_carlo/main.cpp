// Copyright 2022 Krainev Alexander
#include <gtest/gtest.h>
#include <cmath>
#include "./monte_carlo.h"
#include <gtest-mpi-listener.hpp>

constexpr double eps = 1e-2;

TEST(MONTE_CARLO, PLUS_1024) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 1024;
    const double a = 2.0;
    const double b = 7.0;
    const double h = 9.0;

    double resultParallel = monteCarloParallel(n, a, b, h, funcPlus);

    if (rank == 0) {
        double resultSequential = monteCarloSequential(n, a, b, h, funcPlus);
        EXPECT_LE(std::abs(resultParallel - resultSequential), eps);
    }
}

TEST(MONTE_CARLO, PLUS_2048) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 2048;
    const double a = 2.0;
    const double b = 7.0;
    const double h = 9.0;

    double resultParallel = monteCarloParallel(n, a, b, h, funcPlus);

    if (rank == 0) {
        double resultSequential = monteCarloSequential(n, a, b, h, funcPlus);
        EXPECT_LE(std::abs(resultParallel - resultSequential), eps);
    }
}

TEST(MONTE_CARLO, MINUS) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 1024;
    const double a = 2.0;
    const double b = 7.0;
    const double h = 9.0;

    double resultParallel = monteCarloParallel(n, a, b, h, funcMinus);

    if (rank == 0) {
        double resultSequential = monteCarloSequential(n, a, b, h, funcMinus);
        EXPECT_LE(std::abs(resultParallel - resultSequential), eps);
    }
}

TEST(MONTE_CARLO, SQRT) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 1024;
    const double a = 2.0;
    const double b = 7.0;
    const double h = 9.0;

    double resultParallel = monteCarloParallel(n, a, b, h, funcSqrt);

    if (rank == 0) {
        double resultSequential = monteCarloSequential(n, a, b, h, funcSqrt);
        EXPECT_LE(std::abs(resultParallel - resultSequential), eps);
    }
}

TEST(MONTE_CARLO, POW) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int n = 1024;
    const double a = 2.0;
    const double b = 7.0;
    const double h = 9.0;

    double resultParallel = monteCarloParallel(n, a, b, h, funcPow);

    if (rank == 0) {
        double resultSequential = monteCarloSequential(n, a, b, h, funcPow);
        EXPECT_LE(std::abs(resultParallel - resultSequential), eps);
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
