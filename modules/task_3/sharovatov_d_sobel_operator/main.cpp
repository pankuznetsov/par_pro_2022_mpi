// Copyright 2022 Sharovatov Daniil
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>

#include "./sobel_operator.h"

TEST(Sobel_Operator_MPI, Test_Sobel_Operator_Even_Proc_Num) {
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int width = 128;
  int height = ProcNum * 32;

  std::vector<std::vector<int>> sourceImage(height, std::vector<int>(width));

  if (ProcRank == 0) {
    sourceImage = getRandomImage(width, height);
  }

  std::vector<int> imageVector = matrixToVector(sourceImage);

  MPI_Bcast(imageVector.data(), width * height, MPI_INT, 0, MPI_COMM_WORLD);

  if (ProcRank != 0) {
    sourceImage = vectorToMatrix(imageVector, height);
  }

  std::vector<std::vector<int>> newImageSequential;
  if (ProcRank == 0) {
    newImageSequential = sobelOperatorSequential(sourceImage);
  }

  std::vector<std::vector<int>> newImageParallel =
      sobelOperatorParallel(sourceImage);

  if (ProcRank == 0) {
    ASSERT_EQ(newImageParallel, newImageSequential);
  }
}

TEST(Sobel_Operator_MPI, Test_Sobel_Operator_Odd_Proc_Num) {
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int width = 128;
  int height = ProcNum * 32 + 1;

  std::vector<std::vector<int>> sourceImage(height, std::vector<int>(width));

  if (ProcRank == 0) {
    sourceImage = getRandomImage(width, height);
  }

  std::vector<int> imageVector = matrixToVector(sourceImage);

  MPI_Bcast(imageVector.data(), width * height, MPI_INT, 0, MPI_COMM_WORLD);

  if (ProcRank != 0) {
    sourceImage = vectorToMatrix(imageVector, height);
  }

  std::vector<std::vector<int>> newImageSequential;
  if (ProcRank == 0) {
    newImageSequential = sobelOperatorSequential(sourceImage);
  }

  std::vector<std::vector<int>> newImageParallel =
      sobelOperatorParallel(sourceImage);

  if (ProcRank == 0) {
    ASSERT_EQ(newImageParallel, newImageSequential);
  }
}

TEST(Sobel_Operator_MPI, Test_Sobel_Operator_Square_Image) {
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int width = 100;
  int height = 100;

  std::vector<std::vector<int>> sourceImage(height, std::vector<int>(width));

  if (ProcRank == 0) {
    sourceImage = getRandomImage(width, height);
  }

  std::vector<int> imageVector = matrixToVector(sourceImage);

  MPI_Bcast(imageVector.data(), width * height, MPI_INT, 0, MPI_COMM_WORLD);

  if (ProcRank != 0) {
    sourceImage = vectorToMatrix(imageVector, height);
  }

  std::vector<std::vector<int>> newImageSequential;
  if (ProcRank == 0) {
    newImageSequential = sobelOperatorSequential(sourceImage);
  }

  std::vector<std::vector<int>> newImageParallel =
      sobelOperatorParallel(sourceImage);

  if (ProcRank == 0) {
    ASSERT_EQ(newImageParallel, newImageSequential);
  }
}

TEST(Sobel_Operator_MPI, Test_Sobel_Operator_Small_Size) {
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int width = 16;
  int height = 8;

  std::vector<std::vector<int>> sourceImage(height, std::vector<int>(width));

  if (ProcRank == 0) {
    sourceImage = getRandomImage(width, height);
  }

  std::vector<int> imageVector = matrixToVector(sourceImage);

  MPI_Bcast(imageVector.data(), width * height, MPI_INT, 0, MPI_COMM_WORLD);

  if (ProcRank != 0) {
    sourceImage = vectorToMatrix(imageVector, height);
  }

  std::vector<std::vector<int>> newImageSequential;
  if (ProcRank == 0) {
    newImageSequential = sobelOperatorSequential(sourceImage);
  }

  std::vector<std::vector<int>> newImageParallel =
      sobelOperatorParallel(sourceImage);

  if (ProcRank == 0) {
    ASSERT_EQ(newImageParallel, newImageSequential);
  }
}

TEST(Sobel_Operator_MPI, Test_Sobel_Operator_Big_Size) {
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int width = 128;
  int height = 80;

  std::vector<std::vector<int>> sourceImage(height, std::vector<int>(width));

  if (ProcRank == 0) {
    sourceImage = getRandomImage(width, height);
  }

  std::vector<int> imageVector = matrixToVector(sourceImage);

  MPI_Bcast(imageVector.data(), width * height, MPI_INT, 0, MPI_COMM_WORLD);

  if (ProcRank != 0) {
    sourceImage = vectorToMatrix(imageVector, height);
  }

  std::vector<std::vector<int>> newImageSequential;
  if (ProcRank == 0) {
    newImageSequential = sobelOperatorSequential(sourceImage);
  }

  std::vector<std::vector<int>> newImageParallel =
      sobelOperatorParallel(sourceImage);

  if (ProcRank == 0) {
    ASSERT_EQ(newImageParallel, newImageSequential);
  }
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
