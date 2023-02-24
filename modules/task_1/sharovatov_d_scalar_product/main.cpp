// Copyright 2022 Sharovatov Daniil
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>

#include "./scalar_product.h"

TEST(Scalar_Product_MPI, Test_Scalar_Product_Size_10) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  const int vectorSize = 10;

  std::vector<int> v1(vectorSize);
  std::vector<int> v2(vectorSize);

  int resSeq, resPar;

  if (ProcRank == 0) {
    v1 = getRandomVector(vectorSize);
    v2 = getRandomVector(vectorSize);
  }

  resPar = getScalarProductParallel(v1, v2);

  if (ProcRank == 0) {
    resSeq = getScalarProductSequential(v1, v2);
    ASSERT_EQ(resSeq, resPar);
  }
}

TEST(Scalar_Product_MPI, Test_Scalar_Product_Size_100) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  const int vectorSize = 100;

  std::vector<int> v1(vectorSize);
  std::vector<int> v2(vectorSize);

  int resSeq, resPar;

  if (ProcRank == 0) {
    v1 = getRandomVector(vectorSize);
    v2 = getRandomVector(vectorSize);

    resSeq = getScalarProductSequential(v1, v2);
  }

  resPar = getScalarProductParallel(v1, v2);

  if (ProcRank == 0) {
    ASSERT_EQ(resSeq, resPar);
  }
}

TEST(Scalar_Product_MPI, Test_Scalar_Product_Large_Size) {
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  const int vectorSize = 10000000;

  std::vector<int> v1(vectorSize);
  std::vector<int> v2(vectorSize);

  int resSeq, resPar;

  if (ProcRank == 0) {
    v1 = getRandomVector(vectorSize);
    v2 = getRandomVector(vectorSize);

    resSeq = getScalarProductSequential(v1, v2);
  }

  resPar = getScalarProductParallel(v1, v2);

  if (ProcRank == 0) {
    ASSERT_EQ(resSeq, resPar);
  }
}

TEST(Scalar_Product_MPI, Test_Scalar_Product_Even_Proc_Num) {
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  const int vectorSize = 10 * ProcNum;

  std::vector<int> v1(vectorSize);
  std::vector<int> v2(vectorSize);

  int resSeq, resPar;

  if (ProcRank == 0) {
    v1 = getRandomVector(vectorSize);
    v2 = getRandomVector(vectorSize);

    resSeq = getScalarProductSequential(v1, v2);
  }

  resPar = getScalarProductParallel(v1, v2);

  if (ProcRank == 0) {
    ASSERT_EQ(resSeq, resPar);
  }
}

TEST(Scalar_Product_MPI, Test_Scalar_Product_Odd_Proc_Num) {
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  const int vectorSize = 10 * ProcNum + 1;

  std::vector<int> v1(vectorSize);
  std::vector<int> v2(vectorSize);

  int resSeq, resPar;

  if (ProcRank == 0) {
    v1 = getRandomVector(vectorSize);
    v2 = getRandomVector(vectorSize);

    resSeq = getScalarProductSequential(v1, v2);
  }

  resPar = getScalarProductParallel(v1, v2);

  if (ProcRank == 0) {
    ASSERT_EQ(resSeq, resPar);
  }
}

TEST(Scalar_Product_MPI, Test_Scalar_Product_With_Different_Size_Par) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  const int vectorSize1 = 10;
  const int vectorSize2 = 100;

  std::vector<int> v1(vectorSize1);
  std::vector<int> v2(vectorSize2);

  int resSeq, resPar;

  if (ProcRank == 0) {
    v1 = getRandomVector(vectorSize1);
    v2 = getRandomVector(vectorSize2);

    ASSERT_ANY_THROW(getScalarProductSequential(v1, v2));
  }
}

TEST(Scalar_Product_MPI, Test_Scalar_Product_With_Different_Size_Seq) {
  int ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  const int vectorSize1 = 10;
  const int vectorSize2 = 100;

  std::vector<int> v1(vectorSize1);
  std::vector<int> v2(vectorSize2);

  int resSeq, resPar;

  if (ProcRank == 0) {
    v1 = getRandomVector(vectorSize1);
    v2 = getRandomVector(vectorSize2);
  }

  ASSERT_ANY_THROW(getScalarProductParallel(v1, v2));
}



int main(int argc, char* argv[]) {
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
