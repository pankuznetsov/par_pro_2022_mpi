// Copyright 2022 me
#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./shell_merge_sort.h"

void test_body(uint32_t size) {
  int32_t rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<float> src;

  if (rank == 0) {
    src = generate_array(size);
  }
  src = merge_sort_par(src);
  if (rank == 0) {
    EXPECT_TRUE(check(src));
  }
}

TEST(solver_test_series, small_size) { test_body(1000); }

TEST(solver_test_series, moderate_size) { test_body(10000); }

TEST(solver_test_series, big_size) { test_body(100000); }

TEST(solver_test_series,
     i_dont_think_an_average_human_can_imagine_such_numbers) {
  test_body(10000000);
}

TEST(solver_test_series, one_element) { test_body(1); }

TEST(solver_test_series, two_elements) { test_body(2); }

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
