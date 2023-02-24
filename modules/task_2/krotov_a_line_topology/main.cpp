// Copyright 2023 Krotov Aleksey
#include <gtest/gtest.h>
#include "./line_topology.h"
#include <gtest-mpi-listener.hpp>

void LineTopology_UniversalTest(int from, int dest, int tag) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> data(5);
    if (rank == from) {
        data[0] = 1;
        data[1] = 2;
        data[2] = 3;
        data[3] = 4;
        data[4] = 5;
    }

    if (size < 2 || from == dest) {
        ASSERT_ANY_THROW(
            LineSend(data.data(), 5, MPI_INT, from, dest, tag, MPI_COMM_WORLD));
    } else if (from >= size || dest >= size) {
        ASSERT_ANY_THROW(
            LineSend(data.data(), 5, MPI_INT, from, dest, tag, MPI_COMM_WORLD));
    } else {
        LineSend(data.data(), 5, MPI_INT, from, dest, tag, MPI_COMM_WORLD);
        if (rank == dest) {
            ASSERT_EQ(data, std::vector<int>({1, 2, 3, 4, 5}));
        }
    }
}

TEST(LineTopology, LT_Test1) { LineTopology_UniversalTest(0, 0, 1); }

TEST(LineTopology, LT_Test2) { LineTopology_UniversalTest(1000, 10000, 2); }

TEST(LineTopology, LT_Test3) { LineTopology_UniversalTest(0, 1, 3); }

TEST(LineTopology, LT_Test4) { LineTopology_UniversalTest(1, 3, 5); }

TEST(LineTopology, LT_Test5) { LineTopology_UniversalTest(3, 1, 6); }

TEST(LineTopology, LT_Test6) { LineTopology_UniversalTest(2, 0, 8); }

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
