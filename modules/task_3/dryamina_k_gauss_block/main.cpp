// Copyright 2022 Dryamina Kseniya
#include <gtest/gtest.h>
#include <vector>
#include "./gauss.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Image_100x1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 8;
    int cols = 1;
    std::vector<double> Image;
    if (rank == 0) {
        Image = getRandomImage(rows, cols);
    }
    std::vector<double> result = getParallelOperations(Image, rows, cols);
    if (rank == 0) {
        std::vector<double> control_result = getSequentialOperations(Image, rows, cols);
        ASSERT_EQ(control_result, result);
    }
}

TEST(Parallel_Operations_MPI, Image_1x100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 1;
    int cols = 8;
    std::vector<double> Image;
    if (rank == 0) {
        Image = getRandomImage(rows, cols);
    }
    std::vector<double> result = getParallelOperations(Image, rows, cols);
    if (rank == 0) {
        std::vector<double> control_result = getSequentialOperations(Image, rows, cols);
        ASSERT_EQ(control_result, result);
    }
}

TEST(Parallel_Operations_MPI, Image_513x234) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 513;
    int cols = 234;
    std::vector<double> Image;
    if (rank == 0) {
        Image = getRandomImage(rows, cols);
    }
    std::vector<double> result = getParallelOperations(Image, rows, cols);
    if (rank == 0) {
        std::vector<double> control_result = getSequentialOperations(Image, rows, cols);
        ASSERT_EQ(control_result, result);
    }
}

TEST(Parallel_Operations_MPI, Image_234x513) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 234;
    int cols = 512;
    std::vector<double> Image;
    if (rank == 0) {
        Image = getRandomImage(rows, cols);
    }
    std::vector<double> result = getParallelOperations(Image, rows, cols);
    if (rank == 0) {
        std::vector<double> control_result = getSequentialOperations(Image, rows, cols);
        ASSERT_EQ(control_result, result);
    }
}

TEST(Parallel_Operations_MPI, Image_512x512) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 512;
    int cols = 512;
    std::vector<double> Image;
    if (rank == 0) {
        Image = getRandomImage(rows, cols);
    }
    double parallel_start = MPI_Wtime();
    std::vector<double> result = getParallelOperations(Image, rows, cols);
    if (rank == 0) {
        std::vector<double> control_result = getSequentialOperations(Image, rows, cols);
        ASSERT_EQ(control_result, result);
    }
}

TEST(Parallel_Operations_MPI, Empty_Image) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> Image;
    int rows = 1;
    int cols = 1;

    ASSERT_ANY_THROW(getParallelOperations(Image, rows, cols));
}

TEST(Parallel_Operations_MPI, Invalid_Param) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> Image;
    int rows = 1;
    int cols = 1;
    if (rank == 0) {
        Image = getRandomImage(rows, cols);
    }

    ASSERT_ANY_THROW(getParallelOperations(Image, rows - 1, cols - 1));
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
