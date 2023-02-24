// Copyright 2023 Kuleva Anna
#include <gtest/gtest.h>
#include "./gauss_filter.h"
#include <gtest-mpi-listener.hpp>


TEST(Gauss_Filter, Image_3x3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Matrix baseImage;
    Matrix expectedImage;

    if (rank == 0) {
        baseImage = {
            1, 2, 3,
            4, 5, 6,
            7, 8, 9
        };

        expectedImage = baseImage;
    }

    Matrix result = applyGauseFilterPar(baseImage, 3, 3);

    if (rank == 0) {
        ASSERT_EQ(expectedImage, result);
    }
}

TEST(Gauss_Filter, Image_4x3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Matrix baseImage;
    Matrix expectedImage;

    if (rank == 0) {
        baseImage = {
            20, 2, 3, 40,
            4, 5, 6, 7,
            7, 80, 9, 10
        };

        expectedImage = {
            20, 2, 3, 40,
            4, 15, 12, 7,
            7, 80, 9, 10
        };
    }

    Matrix result = applyGauseFilterPar(baseImage, 4, 3);

    if (rank == 0) {
        ASSERT_EQ(expectedImage, result);
    }
}

TEST(Gauss_Filter, Image_3x4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Matrix baseImage;
    Matrix expectedImage;

    if (rank == 0) {
        baseImage = {
            1, 2, 3,
            40, 5, 6,
            7, 8, 9,
            10, 11, 120
        };

        expectedImage = {
            1, 2, 3,
            40, 9, 6,
            7, 17, 9,
            10, 11, 120
        };
    }

    Matrix result = applyGauseFilterPar(baseImage, 3, 4);

    if (rank == 0) {
        ASSERT_EQ(expectedImage, result);
    }
}

TEST(Gauss_Filter, Image_4x4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Matrix baseImage;
    Matrix expectedImage;

    if (rank == 0) {
        baseImage = {
            1, 2, 3, 4,
            5, 6, 7, 80,
            90, 10, 11, 12,
            13, 14, 15, 16
        };

        expectedImage = {
            1, 2, 3, 4,
            5, 11, 16, 80,
            90, 20, 15, 12,
            13, 14, 15, 16
        };
    }

    Matrix result = applyGauseFilterPar(baseImage, 4, 4);

    if (rank == 0) {
        ASSERT_EQ(expectedImage, result);
    }
}

TEST(Gauss_Filter, Image_5x4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Matrix baseImage;
    Matrix expectedImage;

    if (rank == 0) {
        baseImage = {
            1, 2, 3, 4, 5,
            5, 6, 7, 80, 6,
            90, 10, 11, 12, 7,
            13, 14, 15, 16, 8
        };

        expectedImage = {
            1, 2, 3, 4, 5,
            5, 11, 16, 25, 6,
            90, 20, 15, 19, 7,
            13, 14, 15, 16, 8
        };
    }

    Matrix result = applyGauseFilterPar(baseImage, 5, 4);

    if (rank == 0) {
        ASSERT_EQ(expectedImage, result);
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
