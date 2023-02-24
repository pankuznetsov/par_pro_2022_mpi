// Copyright 2023 Pankov Anatoliy
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./allreduce.h"

TEST(Allreduce_Tests, Test_IntMatrix_20) {
    int comm_sz;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    const int columnsCount = 20;

    int* send = new int[columnsCount];
    int* recv = new int[columnsCount];
    for (int i = 0; i < columnsCount; ++i) {
        send[i] = 1;
    }

    MyAllreduce(send, recv, columnsCount, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    for (int i = 0; i < columnsCount; ++i) {
        ASSERT_EQ(recv[i], comm_sz);
    }
}

TEST(Allreduce_Tests, Test_FloatMatrix_20) {
    int comm_sz;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    const int columnsCount = 20;

    float* send = new float[columnsCount];
    float* recv = new float[columnsCount];

    for (int i = 0; i < columnsCount; ++i) {
        send[i] = static_cast<float>(2.);
    }

    MyAllreduce(send, recv, columnsCount, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

    for (int i = 0; i < columnsCount; ++i) {
        ASSERT_EQ(recv[i], (float)2. * comm_sz);
    }
}

TEST(Allreduce_Tests, Test_DoubleMatrix_20) {
    int comm_sz;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    const int columnsCount = 20;

    double* send = new double[columnsCount];
    double* recv = new double[columnsCount];

    for (int i = 0; i < columnsCount; ++i) {
        send[i] = 3.0;
    }

    MyAllreduce(send, recv, columnsCount, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    for (int i = 0; i < columnsCount; ++i) {
        ASSERT_EQ(recv[i], comm_sz * 3.);
    }
}

TEST(Allreduce_Tests, Test_IntMatrix_1000) {
    int comm_sz;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    const int columnsCount = 10000;

    int* send = new int[columnsCount];
    int* recv = new int[columnsCount];

    for (int i = 0; i < columnsCount; ++i) {
        send[i] = 1;
    }

    MyAllreduce(send, recv, columnsCount, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    for (int i = 0; i < columnsCount; ++i) {
        ASSERT_EQ(recv[i], comm_sz);
    }
}

TEST(Allreduce_Tests, Test_Zero_Count_AnyThrow) {
    int* recv = new int[5];
    int* send = new int[5];

    for (int i = 0; i < 5; ++i) {
        send[i] = 1;
    }

    ASSERT_ANY_THROW(
        MyAllreduce(send, recv, 0, MPI_INT, MPI_SUM, MPI_COMM_WORLD));
}

TEST(Allreduce_Tests, Test_Null_Buffer_AnyThrow) {
    int* recv = new int[5];
    int* send = nullptr;

    ASSERT_ANY_THROW(
        MyAllreduce(send, recv, 0, MPI_INT, MPI_SUM, MPI_COMM_WORLD));
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
