// Copyright 2023 Anoschenkova Anna
#include <gtest/gtest.h>
#include <vector>

#include "./mat_cols_min.h"
#include <gtest-mpi-listener.hpp>

void randomMatrixColsMinTest(int m, int n) {
    int rank;
    // Matrix is stored "column by column"
    std::vector<int> myMat;
    std::vector<int> rSeq;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        // (Matrix is fully filled only at root process)
        myMat = randColMat(m, n);
        rSeq = minByColsSequential(m, n, myMat);
        // std::cout << "We have this matrix:\n";
        // vecShow(globMat);
        // std::cout << "\nSequential alg result:";
        // vecShow(rSeq);
    }

    std::vector<int> rPar = minByColsParallel(m, n, myMat);

    if (rank == 0) {
        // std::cout << "\nParallel alg result:";
        // vecShow(rPar);
        ASSERT_EQ(rPar, rSeq);
    }
}

TEST(MatColsMin, test_1) { randomMatrixColsMinTest(4, 4); }

TEST(MatColsMin, test_2) { randomMatrixColsMinTest(5, 4); }

TEST(MatColsMin, test_3) { randomMatrixColsMinTest(9, 12); }

TEST(MatColsMin, test_4) { randomMatrixColsMinTest(15, 15); }

TEST(MatColsMin, test_5) { randomMatrixColsMinTest(100, 100); }

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
