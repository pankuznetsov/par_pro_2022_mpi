// Copyright 2022 Dryamina Kseniya
#include <gtest/gtest.h>
#include "./farm.h"
#include <gtest-mpi-listener.hpp>

TEST(Farm_Topology, Exception) {
    ASSERT_NO_THROW(createFarm(MPI_COMM_WORLD));
}

TEST(Farm_Topology, Left_From_Start) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm farm = createFarm(MPI_COMM_WORLD);

    const int direction = 0;
    const int disp = -1;
    int rank_source, rank_dest;

    if (rank == 0) {
        MPI_Cart_shift(farm, direction, disp, &rank_source, &rank_dest);
        ASSERT_EQ(rank_dest, MPI_PROC_NULL);
    }
}

TEST(Farm_Topology, Right_From_End) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm farm = createFarm(MPI_COMM_WORLD);

    const int direction = 0;
    const int disp = 1;
    int rank_source, rank_dest;

    if (rank == size - 1) {
        MPI_Cart_shift(farm, direction, disp, &rank_source, &rank_dest);
        ASSERT_EQ(rank_dest, MPI_PROC_NULL);
    }
}

TEST(Farm_Topology, Right_From_Start) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm farm = createFarm(MPI_COMM_WORLD);

    const int direction = 0;
    const int disp = 1;
    int rank_source, rank_dest;

    if (rank == 0) {
        MPI_Cart_shift(farm, direction, disp, &rank_source, &rank_dest);
        ASSERT_EQ(rank_dest, size == 1 ? MPI_PROC_NULL : 1);
    }
}

TEST(Farm_Topology, Left_From_End) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Comm farm = createFarm(MPI_COMM_WORLD);

    const int direction = 0;
    const int disp = -1;
    int rank_source, rank_dest;

    if (rank == size - 1) {
        MPI_Cart_shift(farm, direction, disp, &rank_source, &rank_dest);
        ASSERT_EQ(rank_dest, size - 2);
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
