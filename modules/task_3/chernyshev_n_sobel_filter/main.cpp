// Copyright 2022 Chernyshev Nikita
#include <gtest/gtest.h>
#include "../../../modules/task_3/chernyshev_n_sobel_filter/sobel_filter.h"
#include <gtest-mpi-listener.hpp>


void TestSobel(int width, int height) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        ByteImage image(width, height);
        image.Randomize();
        ByteImage res_seq = SobelSequential(image);
        ByteImage res_paral = SobelParallel(image);
        ASSERT_EQ(res_seq, res_paral);
    } else {
        SobelParallel(ByteImage(1, 1));
    }
}

TEST(Sobel_Filter_Mpi, Test_1) { TestSobel(10, 5); }

TEST(Sobel_Filter_Mpi, Test_2) { TestSobel(5, 10); }

TEST(Sobel_Filter_Mpi, Test_3) { TestSobel(50, 100); }

TEST(Sobel_Filter_Mpi, Test_4) { TestSobel(100, 50); }

TEST(Sobel_Filter_Mpi, Test_5) { TestSobel(150, 150); }

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
