// Copyright 2022 Tuzhilkina Polina
#include <gtest/gtest.h>
#include <vector>
#include <functional>
#include "./star_top.h"
#include <gtest-mpi-listener.hpp>

void TestStar(int from, int dest) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if ((size < 2) || (size >= from || size >= dest)) return;

    std::vector<int> data(3);

    if (rank == from) {
        data[0] = 1, data[1] = 2, data[2] = 3;
    }
    StarSend(data.data(), 3, MPI_INT, from, dest, 0, MPI_COMM_WORLD);

    if (rank == dest) {
        ASSERT_EQ(data, std::vector<int>({2, 4, 6}));
    }
}

TEST(StarTests, StarTest_1) {
    // 0 -> 1
    TestStar(0, 1);
}

TEST(StarTests, StarTest_2) {
    // 0 -> 2
    TestStar(0, 2);
}

TEST(StarTests, StarTest_3) {
    // 1 -> 0
    TestStar(1, 0);
}

TEST(StarTests, StarTest_4) {
    // 2 -> 0
    TestStar(2, 0);
}

TEST(StarTests, StarTest_5) {
    // 1 -> 0 -> 2
    TestStar(1, 2);
}

TEST(StarTests, StarTest_6) {
    // 2 -> 0 -> 3
    TestStar(2, 3);
}

TEST(StarTests, StarTest_7) { TestStar(3, 1); }

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
