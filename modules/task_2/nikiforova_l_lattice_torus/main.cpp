// Copyright 2022 Nikiforova Lada
#include <gtest/gtest.h>

#include <cmath>
#include <gtest-mpi-listener.hpp>

#include "./lattice_torus.h"

TEST(Parallel_Operations_MPI, Test_0_send_to_last) {
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int message = 101;
    int last = size - 1;
    int result = optimalRoute(message, 0, last);

    if (rank == size - 1) {
        ASSERT_EQ(message, result);
    }
}

TEST(Parallel_Operations_MPI, Test_last_send_to_0) {
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int message = 11;
    int last = size - 1;
    int result = optimalRoute(message, last, 0);

    if (rank == 0) {
        ASSERT_EQ(message, result);
    }
}

TEST(Parallel_Operations_MPI, Test_0_send_to_avarage) {
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int message = 110;
    int last = size - 1;
    int avarage = floor(last / 2);
    int result = optimalRoute(message, 0, avarage);

    if (rank == avarage) {
        ASSERT_EQ(message, result);
    }
}

TEST(Parallel_Operations_MPI, Test_0_avarage_to_last) {
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int message = 999;
    int last = size - 1;
    int avarage = floor(last / 2);
    int result = optimalRoute(message, avarage, last);

    if (rank == last) {
        ASSERT_EQ(message, result);
    }
}

TEST(Parallel_Operations_MPI, Test_thrid_to_avarage) {
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int message = 666;
    int last = size - 1;
    int avarage = floor(last / 2);
    int third = floor(last / 3);
    int result = optimalRoute(message, third, avarage);

    if (rank == avarage) {
        ASSERT_EQ(message, result);
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
