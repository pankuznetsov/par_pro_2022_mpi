// Copyright 2022 me
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./gauss_by_columns.h"

void test_body(uint32_t size_x, uint32_t size_y) {
  int32_t rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  bool delta_check = false;

  Gaussian_Solver solver(size_x, size_y);

  std::vector<float> seq_ans;
  std::vector<float> par_ans;

  if (rank == 0) {
    solver.generate_matrix();
    seq_ans = solver.solve_sequential();
    float seq_delta = solver.check_answer(seq_ans);
    delta_check = seq_delta <= 1.f;
    EXPECT_TRUE(delta_check);
  }
  par_ans = solver.solve_parallel();
  if (rank == 0) {
    float par_delta = solver.check_answer(par_ans);
    delta_check &= par_delta <= 1.f;
    EXPECT_TRUE(delta_check);
  }
}

TEST(solver_test_series, big_matrix_test_x_is_even) { test_body(1000, 999); }

TEST(solver_test_series, moderate_matrix_test_x_is_even) { test_body(100, 99); }

TEST(solver_test_series, small_matrix_test_x_is_even) { test_body(10, 9); }

TEST(solver_test_series, big_matrix_test_x_is_odd) { test_body(1001, 1000); }

TEST(solver_test_series, moderate_matrix_test_x_is_odd) { test_body(101, 100); }

TEST(solver_test_series, small_matrix_test_x_is_odd) { test_body(11, 10); }

TEST(solver_test_series, smallest_matrix_test) { test_body(15, 14); }

int main(int argc, char** argv) {
  // Filter out Google Test arguments
  ::testing::InitGoogleTest(&argc, argv);

  // Initialize MPI
  MPI_Init(&argc, &argv);

  // Add object that will finalize MPI on exit; Google Test owns this pointer
  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);

  // Get the event listener list.
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  // Remove default listener
  delete listeners.Release(listeners.default_result_printer());

  // Adds MPI listener; Google Test owns this pointer
  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);

  // Run tests, then clean up and exit. RUN_ALL_TESTS() returns 0 if all tests
  // pass and 1 if some test fails.
  return RUN_ALL_TESTS();
}
