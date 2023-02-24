// Copyright 2022 Tolkachev Nikita
#include <gtest/gtest.h>
#include <mpi.h>
#include "../../../modules/task_1/tolkachev_n_matrix_el_sum/matrix_el_sum.h"
#include <gtest-mpi-listener.hpp>
void test_function(int m, int n) {
    int res1, res2 = 0;
    int size = m * n;
    int rank;
    std::vector<int> matrix(size, 0);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        matrix = getRandomMatrix(m, n);
    }
    res2 = getParallelSum(matrix);
    if (rank == 0) {
        res1 = getVectorSum(matrix);
        ASSERT_EQ(res1, res2);
    }
}

TEST(Sum_of_matrix_elements_MPI, Test_1) {
    int m = 1;
    int n = 2;
    test_function(m, n);
}

TEST(Sum_of_matrix_elements_MPI, Test_2) {
    int m = 3;
    int n = 2;
    test_function(m, n);
}

TEST(Sum_of_matrix_elements_MPI, Test_3) {
    int m = 5;
    int n = 5;
    test_function(m, n);
}

TEST(Sum_of_matrix_elements_MPI, Test_4) {
    int m = 3;
    int n = 4;
    test_function(m, n);
}

TEST(Sum_of_matrix_elements_MPI, Test_5) {
    int m = 10;
    int n = 10;
    test_function(m, n);
}

TEST(Sum_of_matrix_elements_MPI, Test_6) {
    int m = 7;
    int n = 8;
    test_function(m, n);
}

TEST(Sum_of_matrix_elements_MPI, Test_7) {
    int m = 1;
    int n = 1;
    test_function(m, n);
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
