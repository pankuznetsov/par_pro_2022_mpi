// Copyright 2023 Makarov Mikhail
#include <gtest/gtest.h>
#include <vector>
#include "./matmult_hor_a.h"
#include <gtest-mpi-listener.hpp>

void MultTest(int m1, int n1, int m2, int n2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> matrixA;
    std::vector<int> matrixB;

    std::vector<int> matrixResPar;
    std::vector<int> matrixResSeq;

    if (n1 != m2) {
        throw "Matrices have ncorrect dimensions!";
    }

    if (rank == 0) {
        matrixA = getRandVector(m1 * n1);
        matrixB = getRandVector(m2 * n2);

        /* std::cout << "MatrixA:\n";
        printAsMatrix(matrixA, m1, n1);
        std::cout << "\n\n";
        std::cout << "MatrixB:\n";
        printAsMatrix(matrixB, m2, n2);
        std::cout << "\n"; */

        matrixResSeq =
            multSequential(matrixA, matrixB, m1, n1, m2, n2);

        /* std::cout << "\n\n";
        std::cout << "MatrixResSeq:\n";
        printAsMatrix(matrixResSeq, m1, n2);
        std::cout << "\n"; */
    }

    matrixResPar = multParallel(matrixA, matrixB, m1, n1, m2, n2);

    if (rank == 0) {
        /* std::cout << "\n";
        std::cout << "MatrixResPar:\n";
        printAsMatrix(matrixResPar, m1, n2);
        std::cout << "\n"; */

        ASSERT_EQ(matrixResSeq, matrixResPar);
    }
}

TEST(MatMultHorA, Test_MatMultHorA_1) {
    MultTest(2, 2, 2, 3);
}

TEST(MatMultHorA, Test_MatMultHorA_2) {
    MultTest(3, 3, 3, 3);
}

TEST(MatMultHorA, Test_MatMultHorA_3) {
    MultTest(6, 3, 3, 5);
}

TEST(MatMultHorA, Test_MatMultHorA_4) {
    MultTest(5, 3, 3, 6);
}

TEST(MatMultHorA, Test_MatMultHorA_5) {
    MultTest(25, 25, 25, 25);
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
