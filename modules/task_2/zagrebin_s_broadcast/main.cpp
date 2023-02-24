// Copyright 2022 Zagrebin S.

#include <gtest/gtest.h>
#include <iostream>
#include "./broadcast.h"
#include <gtest-mpi-listener.hpp>

TEST(Broadcast_MPI, Check_test) {
    int rank, data = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        data = 42;
    MyBcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);

    ASSERT_EQ(data, 42);
}

TEST(Broadcast_MPI, Big_data_test) {
    int rank, *data = new int[100];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        data[42] = 24;
    MyBcast(data, 100, MPI_INT, 0, MPI_COMM_WORLD);

    ASSERT_EQ(data[42], 24);
    delete[] data;
}

TEST(Broadcast_MPI, Double_type_test) {
    int rank; double data = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        data = 42.5;
    MyBcast(&data, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    ASSERT_EQ(data, 42.5);
}

TEST(Broadcast_MPI, Custom_communicator_test) {
    int rank, data = 0; MPI_Comm comm;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int color = rank % 2;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &comm);
    if (rank == 0)
        data = 42;
    MyBcast(&data, 1, MPI_INT, 0, comm);

    if (color == 0)
        ASSERT_EQ(data, 42);
}

TEST(Broadcast_MPI, Time_test) {
    int rank, data; double t1, t2;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        data = 10000;
        t1 = MPI_Wtime();
    }
    MyBcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    for (; data > 0; data = data - 1)
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        t2 = MPI_Wtime();
        std::cout << "Custom Broadcast: " << t2 - t1 << '\n';
    }
    ASSERT_EQ(data == 0, true);

    if (rank == 0) {
        data = 10000;
        t1 = MPI_Wtime();
    }
    MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    for (; data > 0; data = data - 1)
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        t2 = MPI_Wtime();
        std::cout << "MPI Broadcast   : " << t2 - t1 << '\n';
    }
    ASSERT_EQ(data == 0, true);
}

TEST(Broadcast_MPI, Big_data_time_test) {
    int rank, *data = new int[100]; double t1, t2;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        data[42] = 24;
        t1 = MPI_Wtime();
    }
    MyBcast(data, 100, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        t2 = MPI_Wtime();
        std::cout << "Custom Broadcast: " << t2 - t1 << '\n';
    }
    ASSERT_EQ(data[42], 24);

    if (rank == 0) {
        data[42] = 56;
        t1 = MPI_Wtime();
    }
    MPI_Bcast(data, 100, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        t2 = MPI_Wtime();
        std::cout << "MPI Broadcast   : " << t2 - t1 << '\n';
    }
    ASSERT_EQ(data[42], 56);
    delete[] data;
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
