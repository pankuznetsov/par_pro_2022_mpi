// Copyright 2022 Kuznetsov Sergey
#include <gtest/gtest.h>
#include <vector>
#include <gtest-mpi-listener.hpp>


#include "./integration2d_trapezzoid.h"
using std::vector;

double tf1(double x, double y) { return 1; }

double tf2(double x, double y) { return x; }

double tf3(double x, double y) { return x + y; }

TEST(INTEGERATION_2D, PARALLEL_IS_EQUIVALENT_TO_SINGLE_THREADED_1) {
  int proc_num, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double mpi = Integrate2D_Trapezoid_Parallel(0, 1, 0, 1, 1000, 1000, tf1);
  if (rank == 0) {
    double no_mpi = Integrate2D_Trapezoid(0, 1, 0, 1, 1000, 1000, tf1);
    ASSERT_NEAR(no_mpi, mpi, 0.2);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(INTEGERATION_2D, PARALLEL_IS_EQUIVALENT_TO_SINGLE_THREADED_2) {
  int proc_num, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double mpi = Integrate2D_Trapezoid_Parallel(0, 1, 0, 1, 100, 100, tf2);
  if (rank == 0) {
    double no_mpi = Integrate2D_Trapezoid(0, 1, 0, 1, 100, 100, tf2);
    ASSERT_NEAR(no_mpi, mpi, 0.2);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(INTEGERATION_2D, TEST_3) {
  int proc_num, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double mpi = Integrate2D_Trapezoid_Parallel(0, 1, 0, 1, 100, 100, tf1);
  if (rank == 0) {
    ASSERT_NEAR(mpi, 1, 0.2);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(INTEGERATION_2D, TEST_4) {
  int proc_num, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double mpi = Integrate2D_Trapezoid_Parallel(0, 1, 0, 1, 100, 100, tf2);
  if (rank == 0) {
    ASSERT_NEAR(mpi, 0.5, 0.2);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(INTEGERATION_2D, TEST_5) {
  int proc_num, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double mpi = Integrate2D_Trapezoid_Parallel(0, 2, 0, 2, 100, 100, tf1);
  if (rank == 0) {
    ASSERT_NEAR(mpi, 4, 0.2);
  }
  MPI_Barrier(MPI_COMM_WORLD);
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
