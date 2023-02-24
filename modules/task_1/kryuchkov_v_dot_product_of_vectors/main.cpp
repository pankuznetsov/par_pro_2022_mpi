// Copyright 2022 Kryuchkov Vladimir
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_1/kryuchkov_v_dot_product_of_vectors/dot_product_of_vectors.h"
#include <gtest-mpi-listener.hpp>

TEST(parallel_mpi, test_scalar_std_1) {
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  std::vector<int> a;
  std::vector<int> b;
  if (proc_rank == 0) {
    a = get_random_vector(100);
    b = get_random_vector(100);
  }
  int par_res = get_parallel_product(a, b);
  if (proc_rank == 0) {
    int seq_res = get_sequential_product_std(a, b);
    ASSERT_EQ(seq_res, par_res);
  }
}

TEST(parallel_mpi, test_scalar_1) {
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  std::vector<int> a;
  std::vector<int> b;
  if (proc_rank == 0) {
    a = get_random_vector(100);
    b = get_random_vector(100);
  }
  int par_res = get_parallel_product(a, b);
  if (proc_rank == 0) {
    int seq_res = get_sequential_product(a, b);
    ASSERT_EQ(seq_res, par_res);
  }
}

TEST(parallel_mpi, test_scalar_std_2) {
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  std::vector<int> a;
  std::vector<int> b;
  if (proc_rank == 0) {
    a = get_random_vector(500);
    b = get_random_vector(500);
  }
  int par_res = get_parallel_product(a, b);
  if (proc_rank == 0) {
    int seq_res = get_sequential_product_std(a, b);
    ASSERT_EQ(seq_res, par_res);
  }
}

TEST(parallel_mpi, test_scalar_2) {
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  std::vector<int> a;
  std::vector<int> b;
  if (proc_rank == 0) {
    a = get_random_vector(500);
    b = get_random_vector(500);
  }
  int par_res = get_parallel_product(a, b);
  if (proc_rank == 0) {
    int seq_res = get_sequential_product(a, b);
    ASSERT_EQ(seq_res, par_res);
  }
}

TEST(parallel_mpi, test_scalar_std_3) {
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  std::vector<int> a;
  std::vector<int> b;
  if (proc_rank == 0) {
    a = get_random_vector(1000);
    b = get_random_vector(1000);
  }
  int par_res = get_parallel_product(a, b);
  if (proc_rank == 0) {
    int seq_res = get_sequential_product_std(a, b);
    ASSERT_EQ(seq_res, par_res);
  }
}

TEST(parallel_mpi, test_scalar_3) {
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  std::vector<int> a;
  std::vector<int> b;
  if (proc_rank == 0) {
    a = get_random_vector(1000);
    b = get_random_vector(1000);
  }
  int par_res = get_parallel_product(a, b);
  if (proc_rank == 0) {
    int seq_res = get_sequential_product(a, b);
    ASSERT_EQ(seq_res, par_res);
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
