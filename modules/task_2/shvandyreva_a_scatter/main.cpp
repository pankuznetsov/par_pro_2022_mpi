// Copyright 2022 Shvandyreva Alina
#include <gtest/gtest.h>
#include <vector>
#include "./scatter.h"
#include <gtest-mpi-listener.hpp>

TEST(SCATTER, _MPI_ERR_COUNT) {
    std::vector<int> send(1);
    std::vector<int> recv(1);
    int error = USER_Scatter(send.data(), 1, MPI_INT, recv.data(), 2, MPI_INT, 0, MPI_COMM_WORLD);
    ASSERT_EQ(error, MPI_ERR_COUNT);
}

TEST(SCATTER, _MPI_ERR_TYPE) {
    std::vector<int> send(1);
    std::vector<int> recv(1);
    int error = USER_Scatter(send.data(), 1, MPI_INT, recv.data(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    ASSERT_EQ(error, MPI_ERR_TYPE);
}

TEST(SCATTER, INT) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int n = 8;
    const int count = n / size;

    std::vector<int> send = { 1, 2, 3, 4, 5, 6, 7, 8 };
    std::vector<int> userRecv(count);
    std::vector<int> mpiRecv(count);

    USER_Scatter(send.data(), count, MPI_INT, userRecv.data(), count, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(send.data(), count, MPI_INT, mpiRecv.data(), count, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ(userRecv, mpiRecv);
    }
}

TEST(SCATTER, FLOAT) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int n = 8;
    const int count = n / size;

    std::vector<float> send = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
    std::vector<float> userRecv(count);
    std::vector<float> mpiRecv(count);

    USER_Scatter(send.data(), count, MPI_FLOAT, userRecv.data(), count, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Scatter(send.data(), count, MPI_FLOAT, mpiRecv.data(), count, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ(userRecv, mpiRecv);
    }
}

TEST(SCATTER, DOUBLE) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const int n = 8;
    const int count = n / size;

    std::vector<double> send = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0 };
    std::vector<double> userRecv(count);
    std::vector<double> mpiRecv(count);

    USER_Scatter(send.data(), count, MPI_DOUBLE, userRecv.data(), count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(send.data(), count, MPI_DOUBLE, mpiRecv.data(), count, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        ASSERT_EQ(userRecv, mpiRecv);
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
