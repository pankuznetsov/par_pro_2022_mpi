// Copyright 2022 Lapin Dmitriy
#include <gtest/gtest.h>
#include <vector>
#include "./ops_mpi.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_Minimum1) {
    std::vector<std::vector<int>> p = CreateMatrix(6, 6);


    int a, b;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {

        a = getSequentialOperations(p);

    }

    b = getParallelOperations(p);

    if (rank == 0) {
        ASSERT_EQ(a, b);
    }
}

TEST(Parallel_Operations_MPI, Test_Minimum2) {
    std::vector<std::vector<int>> p = CreateMatrix(50, 50);


    int a, b;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {

        a = getSequentialOperations(p);

    }

    b = getParallelOperations(p);

    if (rank == 0) {
        ASSERT_EQ(a, b);
    }
}

TEST(Parallel_Operations_MPI, Test_Minimum3) {
    std::vector<std::vector<int>> p = CreateMatrix(100, 100);


    int a, b;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {

        a = getSequentialOperations(p);

    }

    b = getParallelOperations(p);

    if (rank == 0) {
        ASSERT_EQ(a, b);
    }
}

TEST(Parallel_Operations_MPI, Test_Minimum4) {
    std::vector<std::vector<int>> p = CreateMatrix(200, 100);


    int a, b;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {

        a = getSequentialOperations(p);

    }

    b = getParallelOperations(p);

    if (rank == 0) {
        ASSERT_EQ(a, b);
    }
}

TEST(Parallel_Operations_MPI, Test_Minimum5) {
    std::vector<std::vector<int>> p = CreateMatrix(300, 400);


    int a, b;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {

        a = getSequentialOperations(p);

    }

    b = getParallelOperations(p);

    if (rank == 0) {
        ASSERT_EQ(a, b);
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
