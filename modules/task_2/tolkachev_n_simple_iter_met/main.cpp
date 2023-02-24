// Copyright 2022 Tolkachev Nikita
#include <gtest/gtest.h>
#include <mpi.h>
#include "../../../modules/task_2/tolkachev_n_simple_iter_met/simple_iter_met.h"
#include <gtest-mpi-listener.hpp>
void test_function1(int n) {
    int rank;
    double eps = 1e-6;
    std::vector<double> A = get_random_matrix_diagonal_dominance(n);
    std::vector<double> b = get_random_vector(n);
    std::vector<double> x_parallel =
        simple_iter_met_parallel(A, b, n, 100, 0.0000000001);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::vector<double> check_solution = matrix_vector_prod(A,
 x_parallel, n);
        for (int i = 0; i < n; i++) ASSERT_NEAR(b[i], check_solution[i], eps);
    }
}
void test_function2(int n) {
    int rank;
    std::vector<double> A = get_random_matrix_diagonal_dominance(n);
    std::vector<double> b = get_random_vector(n);
    std::vector<double> x_parallel =
        simple_iter_met_parallel(A, b, n, 100, 0.0000000001);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        std::vector<double> x_seq =
            simple_iter_met_sequential(A, b, n, 100, 0.0000000001);
        for (int i = 0; i < n; i++) ASSERT_DOUBLE_EQ(x_seq[i], x_parallel[i]);
    }
}

TEST(Simple_iter_met_MPI, Test_1) { test_function2(5); }

TEST(Simple_iter_met_MPI, Test_2) { test_function2(10); }

TEST(Simple_iter_met_MPI, Test_3) { test_function2(20); }

TEST(Simple_iter_met_MPI, Test_4) { test_function2(50); }

TEST(Simple_iter_met_MPI, Test_5) { test_function2(100); }

TEST(Simple_iter_met_MPI, Test_6) { test_function1(10); }

TEST(Simple_iter_met_MPI, Test_7) { test_function1(50); }

TEST(Simple_iter_met_MPI, Test_8) { test_function1(100); }

TEST(Simple_iter_met_MPI, Test_9) { test_function1(500); }

TEST(Simple_iter_met_MPI, Test_10) { test_function1(1000); }

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
