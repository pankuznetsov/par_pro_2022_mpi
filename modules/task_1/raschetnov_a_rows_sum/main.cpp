// Copyright 2023 Raschetnov Alexei
#include <gtest/gtest.h>
#include <vector>
#include "./rows_sum.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, test_even_rows) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_matrix;

  int rows = 300;
  int columns = 200;

  if (rank == 0)
    global_matrix = getRandomMatrix(rows, columns);

  std::vector<int> global_result;
  global_result = getParallelSum(global_matrix, rows, columns);

  if (rank == 0) {
    std::vector<int> reference_result = getSequentialSum(global_matrix, rows, columns);
    ASSERT_EQ(reference_result, global_result);
  }
}

TEST(Parallel_Operations_MPI, test_odd_rows) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_matrix;

  int rows = 301;
  int columns = 200;

  if (rank == 0)
    global_matrix = getRandomMatrix(rows, columns);

  std::vector<int> global_result;
  global_result = getParallelSum(global_matrix, rows, columns);

  if (rank == 0) {
    std::vector<int> reference_result = getSequentialSum(global_matrix, rows, columns);
    ASSERT_EQ(reference_result, global_result);
  }
}

TEST(Parallel_Operations_MPI, test_one_row) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_matrix;

  int rows = 1;
  int columns = 200;

  if (rank == 0)
    global_matrix = getRandomMatrix(rows, columns);

  std::vector<int> global_result;
  global_result = getParallelSum(global_matrix, rows, columns);

  if (rank == 0) {
    std::vector<int> reference_result = getSequentialSum(global_matrix, rows, columns);
    ASSERT_EQ(reference_result, global_result);
  }
}

TEST(Parallel_Operations_MPI, test_many_rows) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_matrix;

  int rows = 500;
  int columns = 200;

  if (rank == 0)
    global_matrix = getRandomMatrix(rows, columns);

  std::vector<int> global_result;
  global_result = getParallelSum(global_matrix, rows, columns);

  if (rank == 0) {
    std::vector<int> reference_result = getSequentialSum(global_matrix, rows, columns);
    ASSERT_EQ(reference_result, global_result);
  }
}

TEST(Parallel_Operations_MPI, test_few_rows) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> global_matrix;

  int rows = 5;
  int columns = 2;

  if (rank == 0)
    global_matrix = getRandomMatrix(rows, columns);

  std::vector<int> global_result;
  global_result = getParallelSum(global_matrix, rows, columns);

  if (rank == 0) {
    std::vector<int> reference_result = getSequentialSum(global_matrix, rows, columns);
    ASSERT_EQ(reference_result, global_result);
  }
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
