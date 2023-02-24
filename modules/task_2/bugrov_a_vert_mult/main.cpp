// Copyright 2022 Bugrov Andrey

#include <gtest/gtest.h>

#include <gtest-mpi-listener.hpp>

#include "./vert_mult.h"

bool AreVectorsEqual(int* one, int* two, int size) {
  int i = 0;
  for (; i < size; i++) {
    if (one[i] != two[i]) {
      return false;
    }
  }
  return true;
}

void TestFunction(const int kN, const int kM, int* matrix, int* result_seq,
                  int* result_par) {
  int i = 0;
  int rank;
  int* rand_vector = new int[kM];
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    RandCreator(rand_vector, kM);
    for (; i < kN; i++) {
      result_seq[i] = 0;
      result_par[i] = 0;
    }
    SeqMult(matrix, rand_vector, result_seq, kN, kM);
  }
  ParMult(matrix, rand_vector, result_par, kN, kM);
}

// random matrix : 313 X 173 or 173 X 313

TEST(vert_mult, regular_square_matr_regular_vector) {
  int i = 0;
  int rank;
  const int kN = 5, kM = 5;
  int matrix[kN * kM];
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    for (i = 0; i < kN * kM; i++) {
      matrix[i] = i + 1;
    }
  }
  int *result_seq = new int[kN], *result_par = new int[kN];
  int* rand_vector = new int[kM];
  RandCreator(rand_vector, kM);
  if (rank == 0) {
    for (i = 0; i < kN; i++) {
      result_seq[i] = 0;
      result_par[i] = 0;
    }
    SeqMult(matrix, rand_vector, result_seq, kN, kM);
  }
  ParMult(matrix, rand_vector, result_par, kN, kM);
  if (rank == 0) {
    EXPECT_EQ(AreVectorsEqual(result_seq, result_par, kN), true);
  }
}
TEST(vert_mult, random_square_matr) {
  const int kN = 227, kM = 227;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int matrix[kN * kM];
  if (rank == 0) {
    RandCreator(matrix, kN * kM);
  }
  int* rand_vector = new int[kM];
  int *result_seq = new int[kN], *result_par = new int[kN];
  TestFunction(kN, kM, matrix, result_seq, result_par);
  if (rank == 0) {
    EXPECT_EQ(AreVectorsEqual(result_seq, result_par, kN), true);
  }
}
TEST(vert_mult, regular_horizontal_matr) {
  const int kN = 4, kM = 6;
  int rank;
  int matrix[kN * kM];
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    for (int i = 0; i < kN * kM; i++) {
      matrix[i] = i + 1;
    }
  }
  int* rand_vector = new int[kM];
  int *result_seq = new int[kN], *result_par = new int[kN];
  TestFunction(kN, kM, matrix, result_seq, result_par);
  if (rank == 0) {
    EXPECT_EQ(AreVectorsEqual(result_seq, result_par, kN), true);
  }
}
TEST(vert_mult, random_horizontal_matr) {
  const int kN = 173, kM = 313;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int matrix[kN * kM];
  if (rank == 0) {
    RandCreator(matrix, kN * kM);
  }
  int* rand_vector = new int[kM];
  int *result_seq = new int[kN], *result_par = new int[kN];
  TestFunction(kN, kM, matrix, result_seq, result_par);
  if (rank == 0) {
    EXPECT_EQ(AreVectorsEqual(result_seq, result_par, kN), true);
  }
}
TEST(vert_mult, regular_vertical_matr) {
  const int kN = 7, kM = 4;
  int rank;
  int matrix[kN * kM];
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    for (int i = 0; i < kN * kM; i++) {
      matrix[i] = i + 1;
    }
  }
  int* rand_vector = new int[kM];
  int *result_seq = new int[kN], *result_par = new int[kN];
  TestFunction(kN, kM, matrix, result_seq, result_par);
  if (rank == 0) {
    EXPECT_EQ(AreVectorsEqual(result_seq, result_par, kN), true);
  }
}
TEST(vert_mult, random_vertical_matr) {
  const int kN = 313, kM = 173;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int matrix[kN * kM];
  if (rank == 0) {
    RandCreator(matrix, kN * kM);
  }
  int* rand_vector = new int[kM];
  int *result_seq = new int[kN], *result_par = new int[kN];
  TestFunction(kN, kM, matrix, result_seq, result_par);
  if (rank == 0) {
    EXPECT_EQ(AreVectorsEqual(result_seq, result_par, kN), true);
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
