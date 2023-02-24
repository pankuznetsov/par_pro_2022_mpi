// Copyright 2022 Kochetov Maksim
#include <gtest/gtest.h>
#include "./lin_stretch.h"
#include <gtest-mpi-listener.hpp>


void runTest(const Bitmap& expected, const Bitmap& source) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const Bitmap result = getLinearStretchingParralel(source);

    if (rank == 0) {
        ASSERT_EQ(expected, result);
    }
}

TEST(Lin_Stretch, Test1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Bitmap source;
    Bitmap expected;

    if (rank == 0) {
        source = Bitmap{
            100, 120, 130,
            140, 110, 150,
            90, 105, 125
        };

        expected = Bitmap{
            42, 127, 170,
            212, 85, 255,
            0, 63, 148
        };
    }

    runTest(expected, source);
}

TEST(Lin_Stretch, Test2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Bitmap source;
    Bitmap expected;

    if (rank == 0) {
        source = Bitmap{
            100, 110, 120,
            130, 140, 150,
            160, 175, 227
        };

        expected = Bitmap{
            0, 20, 40,
            60, 80, 100,
            120, 150, 255
        };
    }

    runTest(expected, source);
}

TEST(Lin_Stretch, Test3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Bitmap source;
    Bitmap expected;

    if (rank == 0) {
        source = genBitmap(130, 100, 227);
        expected = getLinearStretchingSequential(source);
    }

    runTest(expected, source);
}

TEST(Lin_Stretch, Test4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Bitmap source;
    Bitmap expected;

    if (rank == 0) {
        source = genBitmap(260, 0, 256);
        expected = source;
    }

    runTest(expected, source);
}

TEST(Lin_Stretch, Test5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Bitmap source;
    Bitmap expected;

    if (rank == 0) {
        source = genBitmap(100, 0, 120);
        expected = getLinearStretchingSequential(source);
    }

    runTest(expected, source);
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
