// Copyright 2022 Reshetnik Yana
#include <gtest/gtest.h>
#include <vector>
#include "./producers_consumers.h"
#include <gtest-mpi-listener.hpp>

TEST(Producers_Consumers, Test_Simple) {
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int count = (size - 1) / 2;

    int rslt = 0;
    if (rank == 0) {
        rslt = task(7);
    } else if (rank < count * 2 + 1) {
        if (rank % 2) {
            produce(0.005);
        } else {
            consume(0.002);
        }
    }

    if (rank == 0) {
        ASSERT_EQ(0, rslt);
    }
}

TEST(Producers_Consumers, Test_Slow_Consumer) {
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int count = (size - 1) / 2;

    int rslt = 0;
    if (rank == 0) {
        rslt = task(7);
    } else if (rank < count * 2 + 1) {
        if (rank % 2) {
            produce(0.001);
        } else {
            consume(0.007);
        }
    }

    if (rank == 0) {
        ASSERT_EQ(0, rslt);
    }
}

TEST(Producers_Consumers, Test_Slow_Producer) {
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int count = (size - 1) / 2;

    int rslt = 0;
    if (rank == 0) {
        rslt = task(7);
    } else if (rank < count * 2 + 1) {
        if (rank % 2) {
            produce(0.007);
        } else {
            consume(0.001);
        }
    }

    if (rank == 0) {
        ASSERT_EQ(0, rslt);
    }
}

TEST(Producers_Consumers, Test_Small_Buffer) {
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int count = (size - 1) / 2;

    int rslt = 0;
    if (rank == 0) {
        rslt = task(3);
    } else if (rank < count * 2 + 1) {
        if (rank % 2) {
            produce(0.002);
        } else {
            consume(0.003);
        }
    }

    if (rank == 0) {
        ASSERT_EQ(0, rslt);
    }
}

TEST(Producers_Consumers, Test_Big_Buffer) {
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int count = (size - 1) / 2;

    int rslt = 0;
    if (rank == 0) {
        rslt = task(100);
    } else if (rank < count * 2 + 1) {
        if (rank % 2) {
            produce(0.002);
        } else {
            consume(0.003);
        }
    }

    if (rank == 0) {
        ASSERT_EQ(0, rslt);
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
