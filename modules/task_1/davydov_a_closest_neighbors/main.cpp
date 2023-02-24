
// Copyright 2022 Davydov Andrey

#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include "./closest_neighbors.h"
#include <gtest-mpi-listener.hpp>

TEST(Vector_Min_MPI, TEST_SIZE_128) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int vecSize = 128;
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(vecSize);
    }

    int parallelResult = closestNeighborsParallel(vec, vecSize);

    if (rank == 0) {
        int sequentialResult = closestNeighborsSequential(vec);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

TEST(Vector_Min_MPI, TEST_SIZE_512) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int vecSize = 512;
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(vecSize);
    }

    int parallelResult = closestNeighborsParallel(vec, vecSize);

    if (rank == 0) {
        int sequentialResult = closestNeighborsSequential(vec);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

TEST(Vector_Min_MPI, TEST_SIZE_1024) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int vecSize = 1024;
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(vecSize);
    }

    int parallelResult = closestNeighborsParallel(vec, vecSize);

    if (rank == 0) {
        int sequentialResult = closestNeighborsSequential(vec);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

TEST(Vector_Min_MPI, TEST_SIZE_2048) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int vecSize = 2048;
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(vecSize);
    }

    int parallelResult = closestNeighborsParallel(vec, vecSize);

    if (rank == 0) {
        int sequentialResult = closestNeighborsSequential(vec);
        ASSERT_EQ(parallelResult, sequentialResult);
    }
}

TEST(Vector_Min_MPI, TEST_SIZE_1000000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int vecSize = 1000000;
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(vecSize);
    }

    int parallelResult = closestNeighborsParallel(vec, vecSize);

    if (rank == 0) {
        int sequentialResult = closestNeighborsSequential(vec);
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
