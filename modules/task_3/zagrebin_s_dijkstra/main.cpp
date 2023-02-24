// Copyright 2022 Zagrebin S.

#include <gtest/gtest.h>
#include "./dijkstra.h"
#include <gtest-mpi-listener.hpp>

TEST(dijkstra_mpi, Check_test) {
    int rank, n = 3, *res = nullptr;
    int m[] = {0, 1, 7,
               1, 0, 2,
               7, 2, 0};
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        res = new int[n];

    Dijkstra(m, n, 0, res);

    if (rank == 0) {
        ASSERT_EQ(res[0], 0);
        ASSERT_EQ(res[1], 1);
        ASSERT_EQ(res[2], 3);
        delete[] res;
    }
}

TEST(dijkstra_mpi, Root_not_in_0_test) {
    int rank, n = 3, *res = nullptr;
    int m[] = {0, 1, 7,
               1, 0, 2,
               7, 2, 0};
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        res = new int[n];

    Dijkstra(m, n, 1, res);

    if (rank == 0) {
        ASSERT_EQ(res[0], 1);
        ASSERT_EQ(res[1], 0);
        ASSERT_EQ(res[2], 2);
        delete[] res;
    }
}

TEST(dijkstra_mpi, Isolated_root_test) {
    int rank, n = 3, *res = nullptr;
    int m[] = {0, 0, 0,
               0, 0, 1,
               0, 1, 0};
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        res = new int[n];

    Dijkstra(m, n, 0, res);

    if (rank == 0) {
        ASSERT_EQ(res[0], 0);
        ASSERT_EQ(res[1], 0);
        ASSERT_EQ(res[2], 0);
        delete[] res;
    }
}

TEST(dijkstra_mpi, Isolated_vertex_test) {
    int rank, n = 3, *res = nullptr;
    int m[] = {0, 1, 0,
               1, 0, 0,
               0, 0, 0};
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        res = new int[n];

    Dijkstra(m, n, 0, res);

    if (rank == 0) {
        ASSERT_EQ(res[0], 0);
        ASSERT_EQ(res[1], 1);
        ASSERT_EQ(res[2], 0);
        delete[] res;
    }
}

TEST(dijkstra_mpi, Large_graph_test) {
    int rank, n = 10, *m = nullptr, *res = nullptr;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        Random::Rand_Init(-1);
        GenSymMatr(&m, n, 0.6, 9);
        res = new int[n];
    }

    Dijkstra(m, n, 0, res);

    if (rank == 0) {
        delete[] m;
        delete[] res;
    }
}

int main(int argc, char** argv) {
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
