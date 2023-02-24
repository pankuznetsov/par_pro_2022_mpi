// Copyright 2023 Anoschenkova Anna
#include <gtest/gtest.h>
#include "./shell_batcher.h"
#include <gtest-mpi-listener.hpp>

TEST(SortBatcher, SortBatcher_Can_Throw_Assert) {
    std::vector<int> testVec(0);
    ASSERT_ANY_THROW(batcherPar(testVec, testVec.size()));
}

TEST(SortBatcher, Can_Not_Throw_Assert) {
    std::vector<int> testVec(10);
    ASSERT_NO_THROW(batcherPar(testVec, testVec.size()));
}

TEST(SortBatcher, SortBatcher_Simple_Vec) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int vecSize = 10;
    std::vector<int> testVec(vecSize);
    if (rank == 0) testVec = {55, 36, 37, 28, 49, 90, 3, 12, 19, 14};

    std::vector<int> batchVec = batcherPar(testVec, vecSize);

    if (rank == 0) {
        testVec = shellSort(testVec);
        ASSERT_EQ(batchVec, testVec);
    }
}

TEST(SortBatcher, SortBatcher_RandomVec) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> testVec;
    const int vecSize = 10000;
    if (rank == 0) testVec = getRandomVector(vecSize);

    std::vector<int> batchVec = batcherPar(testVec, vecSize);

    if (rank == 0) {
        testVec = shellSort(testVec);
        ASSERT_EQ(batchVec, testVec);
    }
}

TEST(SortBatcher, SortBatcher_Not_Equal) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int vecSize = 10;
    std::vector<int> testVec(vecSize);
    if (rank == 0) testVec = {55, 36, 37, 28, 49, 90, 3, 12, 19, 14};

    std::vector<int> batchVec = batcherPar(testVec, vecSize);

    if (rank == 0) {
        testVec = shellSort(testVec);
        testVec[0] = -1;
        ASSERT_NE(batchVec, testVec);
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
