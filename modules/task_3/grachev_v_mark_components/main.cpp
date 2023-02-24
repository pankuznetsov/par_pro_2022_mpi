// Copyright 2022 Grachev Valentin

#include <gtest/gtest.h>
#include "../../../modules/task_3/grachev_v_mark_components/mark_components.h"
#include <gtest-mpi-listener.hpp>


void TestMarking(int width, int height) {
    int rank;
    double time;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        Image image(width, height);
        image.FillRandomBinary();
        Image result = MarkingParallel(image, &time);
        ASSERT_TRUE(CheckMarking(result));
    } else {
        Image dum(1, 1);
        MarkingParallel(dum, &time);
    }
}

TEST(Mark_Components_Mpi, Test1_3x3) { TestMarking(3, 3); }

TEST(Mark_Components_Mpi, Test2_50x10) { TestMarking(50, 10); }

TEST(Mark_Components_Mpi, Test3_10x50) { TestMarking(10, 50); }

TEST(Mark_Components_Mpi, Test4_50x50) { TestMarking(50, 50); }

TEST(Mark_Components_Mpi, Test5_100x100) { TestMarking(100, 100); }

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
