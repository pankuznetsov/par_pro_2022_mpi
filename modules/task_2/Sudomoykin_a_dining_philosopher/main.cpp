// Copyright 2023 Andrey Sudomoykin

#include <gtest/gtest.h>

#include "./dining_philos.h"

#include <gtest-mpi-listener.hpp>

TEST(Dining_philos_MPI, One_pizza) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & size);
  int runs = 1;

  start(rank, size, runs);

  MPI_Barrier(MPI_COMM_WORLD);

  ASSERT_NO_THROW();
}

TEST(Dining_philos_MPI, Three_pizzas) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & size);
  int runs = 3;

  start(rank, size, runs);

  MPI_Barrier(MPI_COMM_WORLD);

  ASSERT_NO_THROW();
}

TEST(Dining_philos_MPI, Five_pizzas) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & size);
  int runs = 5;

  start(rank, size, runs);

  MPI_Barrier(MPI_COMM_WORLD);

  ASSERT_NO_THROW();
}

TEST(Dining_philos_MPI, Ten_pizzas) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & size);
  int runs = 10;

  start(rank, size, runs);

  MPI_Barrier(MPI_COMM_WORLD);

  ASSERT_NO_THROW();
}

TEST(Dining_philos_MPI, Twenty_pizzas) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & size);
  int runs = 20;

  start(rank, size, runs);

  MPI_Barrier(MPI_COMM_WORLD);

  ASSERT_NO_THROW();
}

int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(& argc, argv);
  MPI_Init(& argc, & argv);

  std::srand(std::time(nullptr));

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners & listeners =
  ::testing::UnitTest::GetInstance() -> listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
