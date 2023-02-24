// Copyright 2022 Grachev Valentin
#include <gtest/gtest.h>
#include "../../../modules/task_2/grachev_v_image_smoothing/image_smoothing.h"
#include <gtest-mpi-listener.hpp>


TEST(Image_Smoothing_Mpi, Image_Smoothing_Test1_3x3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double paral_time, seq_time;

    if (rank == 0) {
        Image origin(3, 3);
        origin.FillRandom();
        Image res_sequential = SmoothingSequential(origin, &seq_time);
        Image res_parallel = SmoothingParallel(origin, &paral_time);
        ASSERT_EQ(res_parallel, res_sequential);
    } else {
        Image dum = Image(1, 1);
        SmoothingParallel(dum, &paral_time);
    }
}

TEST(Image_Smoothing_Mpi, Image_Smoothing_Test2_10x10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double paral_time, seq_time;

    if (rank == 0) {
        Image origin(10, 10);
        origin.FillRandom();
        Image res_sequential = SmoothingSequential(origin, &seq_time);
        Image res_parallel = SmoothingParallel(origin, &paral_time);
        ASSERT_EQ(res_parallel, res_sequential);

    } else {
        Image dum = Image(1, 1);
        SmoothingParallel(dum, &paral_time);
    }
}

TEST(Image_Smoothing_Mpi, Image_Smoothing_Test3_20x300) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double paral_time, seq_time;

    if (rank == 0) {
        Image origin(20, 300);
        origin.FillRandom();
        Image res_sequential = SmoothingSequential(origin, &seq_time);
        Image res_parallel = SmoothingParallel(origin, &paral_time);
        ASSERT_EQ(res_parallel, res_sequential);

    } else {
        Image dum = Image(1, 1);
        SmoothingParallel(dum, &paral_time);
    }
}

TEST(Image_Smoothing_Mpi, Image_Smoothing_Test4_100x50) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double paral_time, seq_time;

    if (rank == 0) {
        Image origin(100, 50);
        origin.FillRandom();
        Image res_sequential = SmoothingSequential(origin, &seq_time);
        Image res_parallel = SmoothingParallel(origin, &paral_time);
        ASSERT_EQ(res_parallel, res_sequential);

    } else {
        Image dum = Image(1, 1);
        SmoothingParallel(dum, &paral_time);
    }
}

TEST(Image_Smoothing_Mpi, Image_Smoothing_Test5_250x250) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double paral_time, seq_time;

    if (rank == 0) {
        Image origin(250, 250);
        origin.FillRandom();
        Image res_sequential = SmoothingSequential(origin, &seq_time);
        Image res_parallel = SmoothingParallel(origin, &paral_time);
        ASSERT_EQ(res_parallel, res_sequential);

    } else {
        Image dum = Image(1, 1);
        SmoothingParallel(dum, &paral_time);
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
