// Copyright 2018 Nesterov Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./simple_iteration.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_SeqSimpleIteration) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int size = 3;
  double accuracy = 0.00001;
  std::vector<double> A = { 10, 2, 5, 4, 12, 3, 7, 3, 13 };
  std::vector<double> b = { 4, 2, 13 };
  if (rank == 0) {
    std::vector<double> ref = { -0.129770992, -0.061068702, 1.083969465 };
    std::vector<double> result = seqSimpleIteration(A, b, accuracy, 50, true);
    ASSERT_NEAR(getAccuracy(result, ref), accuracy, accuracy);
  }
}

TEST(Parallel_Operations_MPI, Test_ParSimpleIteration) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int size = 3;
  double accuracy = 0.00001;
  std::vector<double> A = { 10, 2, 5, 4, 12, 3, 7, 3, 13 };
  std::vector<double> b = { 4, 2, 13 };
  std::vector<double> ref = { -0.129770992, -0.061068702, 1.083969465 };
  std::vector<double> result = parSimpleIteration(A, b, size, accuracy, 50, true);
  if (rank == 0) {
    ASSERT_NEAR(getAccuracy(result, ref), accuracy, accuracy);
  }
}

TEST(Parallel_Operations_MPI, Test_ParSimpleIteration1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int size = 5;
  double accuracy = 0.00001;
  std::vector<double> A;
  std::vector<double> b;
  if (rank == 0) {
    getRandMatrVect(size, -10, 10, &A, &b);
  }
  std::vector<double> result = parSimpleIteration(A, b, size, accuracy, 50, true);
  if (rank == 0) {
    ASSERT_NEAR(checkSolution(size, A, b, result), 0.0, accuracy);
  }
}

TEST(Parallel_Operations_MPI, Test_ParSimpleIteration2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int size = 12;
  double accuracy = 0.00001;
  std::vector<double> A;
  std::vector<double> b;
  if (rank == 0) {
    getRandMatrVect(size, -10, 10, &A, &b);
  }
  std::vector<double> result = parSimpleIteration(A, b, size, accuracy, 50, true);
  if (rank == 0) {
    ASSERT_NEAR(checkSolution(size, A, b, result), 0.0, accuracy);
  }
}

TEST(Parallel_Operations_MPI, Test_ParSimpleIteration3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int size = 33;
  double accuracy = 0.00001;
  std::vector<double> A;
  std::vector<double> b;
  if (rank == 0) {
    getRandMatrVect(size, -10, 10, &A, &b);
  }
  std::vector<double> result = parSimpleIteration(A, b, size, accuracy, 50, true);
  if (rank == 0) {
    ASSERT_NEAR(checkSolution(size, A, b, result), 0.0, accuracy);
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
