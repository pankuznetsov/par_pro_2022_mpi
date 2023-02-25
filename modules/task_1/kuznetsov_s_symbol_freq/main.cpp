// Copyright 2023 Kuznetsov Sergey
#include <gtest/gtest.h>
#include <vector>
#include "./symbol_freq.h"
#include <gtest-mpi-listener.hpp>

/* TEST(Parallel_Operations_MPI, Test_Sum) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_sum = getParallelOperations(global_vec, count_size_vector, "+");

    if (rank == 0) {
        int reference_sum = getSequentialOperations(global_vec, "+");
        ASSERT_EQ(reference_sum, global_sum);
    }
}

TEST(Parallel_Operations_MPI, Test_Diff) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_diff = getParallelOperations(global_vec, count_size_vector, "-");

    if (rank == 0) {
        int reference_diff = getSequentialOperations(global_vec, "-");
        ASSERT_EQ(reference_diff, global_diff);
    }
}

TEST(Parallel_Operations_MPI, Test_Diff_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_diff = getParallelOperations(global_vec, count_size_vector, "-");

    if (rank == 0) {
        int reference_diff = getSequentialOperations(global_vec, "-");
        ASSERT_EQ(reference_diff, global_diff);
    }
}

TEST(Parallel_Operations_MPI, Test_Max) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> global_vec;
    const int count_size_vector = 120;

    if (rank == 0) {
        global_vec = getRandomVector(count_size_vector);
    }

    int global_max;
    global_max = getParallelOperations(global_vec, count_size_vector, "max");

    if (rank == 0) {
        int reference_max = getSequentialOperations(global_vec, "max");
        ASSERT_EQ(reference_max, global_max);
    }
} */

TEST(Sequential_Operations_MPI, Count_A_letters) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string testString = "aabbaaaccaa";
    ASSERT_EQ(symbolFrequencySequential(testString, 'a'), 7);
}

TEST(Parallel_Operations_MPI, Count_A_letters) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string testString = "aabbaaaccaa";
  int res = symbolFrequencyParallel(testString, 'a');
  ASSERT_EQ(res, 7);
}

TEST(Parallel_Operations_MPI, Ensure_equal_result) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string testString = "aabbaaccaa";
  ASSERT_EQ(symbolFrequencyParallel(testString, 'a'), symbolFrequencySequential(testString, 'a'));
}

TEST(Sequential_Operations_MPI, Empty_line) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string testString = "";
  ASSERT_EQ(symbolFrequencySequential(testString, 'a'), 0);
}

TEST(Parallel_Operations_MPI, Empty_line) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string testString = "";
  ASSERT_EQ(symbolFrequencyParallel(testString, 'a'), 0);
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
