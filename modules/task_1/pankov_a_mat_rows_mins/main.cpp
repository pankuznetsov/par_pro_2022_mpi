// Copyright 2023 Pankov Anatoliy
#include <gtest/gtest.h>
#include <vector>
#include "./mat_rows_mins.h"
#include <gtest-mpi-listener.hpp>

TEST(Mat_rows_mins, Test_1_Mat_rows_mins) {
    int m = 2, n = 3;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const bool root_process = (rank == 0);
    std::vector<int> rand_mat;
    std::vector<int> result_sequential;
    std::vector<int> result_parallel;

    if (root_process) {
        rand_mat = random_vec_gen(m * n);
        result_sequential = sequential_mat_rows_mins(m, n, rand_mat);
    }

    result_parallel = parallel_mat_rows_mins(m, n, rand_mat);

    if (root_process) {
        ASSERT_EQ(result_sequential, result_parallel);
    }
}

TEST(Mat_rows_mins, Test_2_Mat_rows_mins) {
    int m = 5, n = 3;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const bool root_process = (rank == 0);
    std::vector<int> rand_mat;
    std::vector<int> result_sequential;
    std::vector<int> result_parallel;

    if (root_process) {
        rand_mat = random_vec_gen(m * n);
        result_sequential = sequential_mat_rows_mins(m, n, rand_mat);
    }

    result_parallel = parallel_mat_rows_mins(m, n, rand_mat);

    if (root_process) {
        ASSERT_EQ(result_sequential, result_parallel);
    }
}

TEST(Mat_rows_mins, Test_3_Mat_rows_mins) {
    int m = 4, n = 4;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const bool root_process = (rank == 0);
    std::vector<int> rand_mat;
    std::vector<int> result_sequential;
    std::vector<int> result_parallel;

    if (root_process) {
        rand_mat = random_vec_gen(m * n);
        result_sequential = sequential_mat_rows_mins(m, n, rand_mat);
    }

    result_parallel = parallel_mat_rows_mins(m, n, rand_mat);

    if (root_process) {
        ASSERT_EQ(result_sequential, result_parallel);
    }
}

TEST(Mat_rows_mins, Test_4_Mat_rows_mins) {
    int m = 10, n = 10;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const bool root_process = (rank == 0);
    std::vector<int> rand_mat;
    std::vector<int> result_sequential;
    std::vector<int> result_parallel;

    if (root_process) {
        rand_mat = random_vec_gen(m * n);
        result_sequential = sequential_mat_rows_mins(m, n, rand_mat);
    }

    result_parallel = parallel_mat_rows_mins(m, n, rand_mat);

    if (root_process) {
        ASSERT_EQ(result_sequential, result_parallel);
    }
}

TEST(Mat_rows_mins, Test_5_Mat_rows_mins) {
    int m = 50, n = 50;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const bool root_process = (rank == 0);
    std::vector<int> rand_mat;
    std::vector<int> result_sequential;
    std::vector<int> result_parallel;

    if (root_process) {
        rand_mat = random_vec_gen(m * n);
        result_sequential = sequential_mat_rows_mins(m, n, rand_mat);
    }

    result_parallel = parallel_mat_rows_mins(m, n, rand_mat);

    if (root_process) {
        ASSERT_EQ(result_sequential, result_parallel);
    }
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
