// Copyright 2018 Nesterov Alexander
#include <gtest/gtest.h>
#include <vector>
#include "./sparse_matrix_mult.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_ConvertToCRS) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> A = { {0, 0, 7, 8},
                                         {3, 0, 0, 0},
                                         {0, 2, 0, 0},
  };
  if (rank == 0) {
    SparseMatrix result(A);
    SparseMatrix ref;
    ref.countRows = 3;
    ref.countColumns = 4;
    ref.values = { 7, 8, 3, 2 };
    ref.columnIndex = { 2, 3, 0, 1 };
    ref.rowIndex = { 0, 2, 3, 4 };
    EXPECT_EQ(result, ref);
  }
}

TEST(Parallel_Operations_MPI, Test_SeqSpareMatrixMult) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> A = { {0, 0, 7, 8},
                                         {3, 0, 0, 0},
                                         {0, 2, 0, 0}
  };
  std::vector<std::vector<double>> B = { {0, 0, 7, 0},
                                         {1, 0, 2, 0},
                                         {0, 2, 0, 2},
                                         {3, 0, 5, 0}
  };
  if (rank == 0) {
    SparseMatrix result = seqSparseMatrixMult(A, B);
    SparseMatrix ref;
    ref.countRows = 3;
    ref.countColumns = 4;
    ref.values = { 24, 14, 40, 14, 21, 2, 4 };
    ref.columnIndex = { 0, 1, 2, 3, 2, 0, 2 };
    ref.rowIndex = { 0, 4, 5, 7 };
    EXPECT_EQ(result, ref);
  }
}

TEST(Parallel_Operations_MPI, Test_ParSpareMatrixMult1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> A = { {0, 0, 7, 8},
                                         {3, 0, 0, 0},
                                         {0, 2, 0, 0}
  };

  std::vector<std::vector<double>> B = { {0, 0, 7, 0},
                                         {1, 0, 2, 0},
                                         {0, 2, 0, 2},
                                         {3, 0, 5, 0}
  };

  SparseMatrix result = parSparseMatrixMult(A, B);
  if (rank == 0) {
    SparseMatrix seqResult = seqSparseMatrixMult(A, B);
    EXPECT_EQ(result, seqResult);
  }
}

TEST(Parallel_Operations_MPI, Test_ParSpareMatrixMult2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> A = { {0, 0, 7, 8},
                                         {3, 0, 0, 0},
                                         {0, 2, 0, 0},
                                         {3, 0, 0, 5},
                                         {1, 0, 10, 0}
  };

  std::vector<std::vector<double>> B = { {0, 0, 7, 0},
                                         {1, 0, 2, 0},
                                         {0, 2, 0, 2},
                                         {3, 0, 5, 0}
  };

  SparseMatrix result = parSparseMatrixMult(A, B);
  if (rank == 0) {
    SparseMatrix seqResult = seqSparseMatrixMult(A, B);
    EXPECT_EQ(result, seqResult);
  }
}

TEST(Parallel_Operations_MPI, Test_ParSpareMatrixMult3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> A = { {1, 0, 0, 0},
                                         {1, 0, 0, 0},
                                         {1, 0, 0, 0},
                                         {1, 0, 0, 0},
                                         {1, 0, 0, 0}
  };

  std::vector<std::vector<double>> B = { {0, 0, 1, 0},
                                         {0, 0, 1, 0},
                                         {0, 0, 1, 0},
                                         {0, 0, 1, 0}
  };

  SparseMatrix result = parSparseMatrixMult(A, B);
  if (rank == 0) {
    SparseMatrix seqResult = seqSparseMatrixMult(A, B);
    EXPECT_EQ(result, seqResult);
  }
}

TEST(Parallel_Operations_MPI, Test_ParSpareMatrixMult4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> A = { {0, 7, 0},
    {0, 0, 0},
    {0, 0, 1}
  };

  std::vector<std::vector<double>> B = { {1, 15, 0},
    {0, 0, 2},
    {0, 0, 0}
  };

  SparseMatrix result = parSparseMatrixMult(A, B);
  if (rank == 0) {
    SparseMatrix seqResult = seqSparseMatrixMult(A, B);
    EXPECT_EQ(result, seqResult);
  }
}

TEST(Parallel_Operations_MPI, Test_ParSpareMatrixMult5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> A = { {1, 0 },
                                         {1, 0 }
  };

  std::vector<std::vector<double>> B = { {0, 5, 0, 0},
                                         {0, 0, 0, 0},
                                         {1, 0, 0, 0},
                                         {0, 0, 7, 0}
  };

  if (rank == 0) {
    ASSERT_ANY_THROW(parSparseMatrixMult(A, B));
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
