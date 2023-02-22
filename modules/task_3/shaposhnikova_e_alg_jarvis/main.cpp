// Copyright 2023 Shaposhnikova Ekaterina

#include <gtest/gtest.h>
#include <cmath>
#include <string>
#include <random>
#include <iostream>
#include "./alg_jarvis.h"
#include <gtest-mpi-listener.hpp>

TEST(jarvis_MPI, Test_1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> pointsVec = {1,2,3,5,3,6,1,7 };
  std::vector<int> parrResult = algJarvisParallel(pointsVec, 4);

  if (rank == 0) {
    std::vector<int> seqResult = algJarvisSequential(pointsVec);
    EXPECT_EQ(parrResult, seqResult);
  }
}

TEST(jarvis_MPI, Test_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> pointsVec = {4,3,2,7,1,8,2,9,10,11,2,2,1,1,5,7};
  std::vector<int> parrResult = algJarvisParallel(pointsVec, 8);

  if (rank == 0) {
    std::vector<int> seqResult = algJarvisSequential(pointsVec);
    EXPECT_EQ(parrResult, seqResult);
  }
}

TEST(jarvis_MPI, Test_3) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> pointsVec = {1,1,2,2,3,3,4,4,8,8};
  std::vector<int> parrResult = algJarvisParallel(pointsVec, 5);

  if (rank == 0) {
    std::vector<int> seqResult = algJarvisSequential(pointsVec);
    EXPECT_EQ(parrResult, seqResult);
  }
}

TEST(jarvis_MPI, Test_4) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> pointsVec = {1,2,3,4,5,6,7,8,9,10,11,12};
  std::vector<int> parrResult = algJarvisParallel(pointsVec, 6);

  if (rank == 0) {
    std::vector<int> seqResult = algJarvisSequential(pointsVec);
    EXPECT_EQ(parrResult, seqResult);
  }
}

TEST(jarvis_MPI, Test_5) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<int> pointsVec = { 1,2,2,5,1,7,3,4,2,6,2,2,5,6,9,1};
  std::vector<int> parrResult = algJarvisParallel(pointsVec,8);

  if (rank == 0) {
    std::vector<int> seqResult = algJarvisSequential(pointsVec);
    EXPECT_EQ(parrResult, seqResult);
  }
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
