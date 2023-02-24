// Copyright 2022 Yarakhtin Anton
#include <gtest/gtest.h>
#include <cmath>
#include <vector>
#include "./topology_torus.h"
#include <gtest-mpi-listener.hpp>

TEST(Topology_Torus, Test_Simple) {
    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size >= 4) {
        Vector2 torus_size = { 2, 2 };

        auto torus = torusCreate(torus_size);

        int data;

        if (rank == 0) {
            data = 7;
        }

        torusSend(&data, 1, MPI_INT, 0, 1, 0, torus);

        if (rank == 1) {
            ASSERT_EQ(data, 7);
        }
    }
}

TEST(Topology_Torus, Test_Line_X) {
    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size >= 4) {
        Vector2 torus_size = { 1, 4 };

        auto torus = torusCreate(torus_size);

        int data;

        if (rank == 0) {
            data = 7;
        }

        torusSend(&data, 1, MPI_INT, 0, 3, 0, torus);

        if (rank == 3) {
            ASSERT_EQ(data, 7);
        }
    }
}

TEST(Topology_Torus, Test_Line_Y) {
    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size >= 4) {
        Vector2 torus_size = { 4, 1 };

        auto torus = torusCreate(torus_size);

        int data;

        if (rank == 0) {
            data = 7;
        }

        torusSend(&data, 1, MPI_INT, 0, 3, 0, torus);

        if (rank == 3) {
            ASSERT_EQ(data, 7);
        }
    }
}

TEST(Topology_Torus, Test_Max_Square_0) {
    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int maxsize = static_cast<int>(sqrt(size));

    if (maxsize >= 2) {
        Vector2 torus_size = { maxsize, maxsize };

        auto torus = torusCreate(torus_size);

        int data;

        if (rank == 0) {
            data = 7;
        }

        torusSend(&data, 1, MPI_INT, 0, maxsize * maxsize - 1, 0, torus);

        if (rank == maxsize * maxsize - 1) {
            ASSERT_EQ(data, 7);
        }
    }
}

TEST(Topology_Torus, Test_Max_Square_1) {
    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int maxsize = static_cast<int>(sqrt(size));

    if (maxsize >= 2) {
        Vector2 torus_size = { maxsize, maxsize };

        auto torus = torusCreate(torus_size);

        int data;

        if (rank == 0) {
            data = 7;
        }

        torusSend(&data, 1, MPI_INT, 0, maxsize - 1, 0, torus);

        if (rank == maxsize - 1) {
            ASSERT_EQ(data, 7);
        }
    }
}

TEST(Topology_Torus, Test_Max_Line) {
    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size >= 2) {
        Vector2 torus_size = { size, 1 };

        auto torus = torusCreate(torus_size);

        int data;

        if (rank == 0) {
            data = 7;
        }

        torusSend(&data, 1, MPI_INT, 0, size / 2, 0, torus);

        if (rank == size / 2) {
            ASSERT_EQ(data, 7);
        }
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
