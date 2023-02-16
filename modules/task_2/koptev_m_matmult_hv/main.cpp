// Copyright 2023 Koptev Maksim
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>

#include "./matmult_hv.h"

void RunTestMult(int matSize) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matA, matB;
    std::vector<int> resultMatParallel, resultMatSeqential;

    if (rank == 0) {
        matA = getRandomMatrix(matSize);
        matB = getRandomMatrix(matSize);
    }
    resultMatParallel =
        getMatrixMultParellel(matA, matB, matSize);

    if (rank == 0) {
        resultMatSeqential =
            getMatrixMultSeq(matA, matB, matSize);

        ASSERT_EQ(resultMatParallel, resultMatSeqential);
    }
}

TEST(TestMult, TestMult_1) { RunTestMult(2); }

TEST(TestMult, TestMult_2) { RunTestMult(5); }

TEST(TestMult, TestMult_3) { RunTestMult(25); }

TEST(TestMult, TestMult_4) { RunTestMult(50); }

TEST(TestMult, TestMult_5) { RunTestMult(100); }

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
