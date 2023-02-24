// Copyright 2022 Kryuchkov Vladimir
#include <gtest/gtest.h>
#include <vector>
#include "./grahams_pass.h"
#include <gtest-mpi-listener.hpp>

TEST(grahams_pass_test, for_10_points) {
  int size = 10;
  std::vector<int> points(0);
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  std::vector<int> shell_par(0);
  int size_shell_par;
  int* size_shell = &size_shell_par;
  if (proc_rank == 0) {
    points = get_points(size, 600, 350, 330);
    shell_par = grahams_parallel(points, size, size_shell);
    int size_shell_seq;
    int* p_sss = &size_shell_seq;
    std::vector<int> shell_seq = grahams_sequential(points, size, p_sss);
    int flag = 1;
    if (size_shell_par != size_shell_seq) {
      flag = 0;
    } else {
      for (int i = 0; i < size_shell_par; i++) {
        if ((shell_par[i * 2] != shell_seq[i * 2]) ||
            (shell_par[i * 2 + 1] != shell_seq[i * 2 + 1])) {
          flag = 0;
          break;
        }
      }
    }
    ASSERT_EQ(flag, 1);
  } else {
    shell_par = grahams_parallel(points, size, size_shell);
  }
}

TEST(graham_pass_test, for_30_points) {
  int size = 30;
  std::vector<int> points(0);
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  std::vector<int> shell_par(0);
  int size_shell_par;
  int* size_shell = &size_shell_par;
  if (proc_rank == 0) {
    points = get_points(size, 600, 350, 330);
    shell_par = grahams_parallel(points, size, size_shell);
    int size_shell_seq;
    int* p_sss = &size_shell_seq;
    std::vector<int> shell_seq = grahams_sequential(points, size, p_sss);
    int flag = 1;
    if (size_shell_par != size_shell_seq) {
      flag = 0;
    } else {
      for (int i = 0; i < size_shell_par; i++) {
        if ((shell_par[i * 2] != shell_seq[i * 2]) ||
            (shell_par[i * 2 + 1] != shell_seq[i * 2 + 1])) {
          flag = 0;
          break;
        }
      }
    }
    ASSERT_EQ(flag, 1);
  } else {
    shell_par = grahams_parallel(points, size, size_shell);
  }
}

TEST(graham_pass_test, for_80_points) {
  int size = 80;
  std::vector<int> points(0);
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  std::vector<int> shell_par(0);
  int size_shell_par;
  int* size_shell = &size_shell_par;
  if (proc_rank == 0) {
    points = get_points(size, 600, 350, 330);
    shell_par = grahams_parallel(points, size, size_shell);
    int size_shell_seq;
    int* p_sss = &size_shell_seq;
    std::vector<int> shell_seq = grahams_sequential(points, size, p_sss);
    int flag = 1;
    if (size_shell_par != size_shell_seq) {
      flag = 0;
    } else {
      for (int i = 0; i < size_shell_par; i++) {
        if ((shell_par[i * 2] != shell_seq[i * 2]) ||
            (shell_par[i * 2 + 1] != shell_seq[i * 2 + 1])) {
          flag = 0;
          break;
        }
      }
    }
    ASSERT_EQ(flag, 1);
  } else {
    shell_par = grahams_parallel(points, size, size_shell);
  }
}

TEST(graham_pass_test, for_100_points) {
  int size = 100;
  std::vector<int> points(0);
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  std::vector<int> shell_par(0);
  int size_shell_par;
  int* size_shell = &size_shell_par;
  if (proc_rank == 0) {
    points = get_points(size, 600, 350, 330);
    shell_par = grahams_parallel(points, size, size_shell);
    int size_shell_seq;
    int* p_sss = &size_shell_seq;
    std::vector<int> shell_seq = grahams_sequential(points, size, p_sss);
    int flag = 1;
    if (size_shell_par != size_shell_seq) {
      flag = 0;
    } else {
      for (int i = 0; i < size_shell_par; i++) {
        if ((shell_par[i * 2] != shell_seq[i * 2]) ||
            (shell_par[i * 2 + 1] != shell_seq[i * 2 + 1])) {
          flag = 0;
          break;
        }
      }
    }
    ASSERT_EQ(flag, 1);
  } else {
    shell_par = grahams_parallel(points, size, size_shell);
  }
}

TEST(graham_pass_test, for_200_points) {
  int size = 200;
  std::vector<int> points(0);
  int proc_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  std::vector<int> shell_par(0);
  int size_shell_par;
  int* size_shell = &size_shell_par;
  if (proc_rank == 0) {
    points = get_points(size, 600, 350, 330);
    shell_par = grahams_parallel(points, size, size_shell);
    int size_shell_seq;
    int* p_sss = &size_shell_seq;
    std::vector<int> shell_seq = grahams_sequential(points, size, p_sss);
    int flag = 1;
    if (size_shell_par != size_shell_seq) {
      flag = 0;
    } else {
      for (int i = 0; i < size_shell_par; i++) {
        if ((shell_par[i * 2] != shell_seq[i * 2]) ||
            (shell_par[i * 2 + 1] != shell_seq[i * 2 + 1])) {
          flag = 0;
          break;
        }
      }
    }
    ASSERT_EQ(flag, 1);
  } else {
    shell_par = grahams_parallel(points, size, size_shell);
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
