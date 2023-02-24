// Copyright 2022 Sharovatov Daniil
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>

#include "./hypercube.h"

TEST(Hypercube_MPI, Test_Is_Hypercube) {
    ASSERT_EQ(isHyperCube(7), false);
}

TEST(Hypercube_MPI, Test_Hypercube_Send_From_0_To_1) {
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if (ProcNum >= 2) {
    int data;
    if (ProcRank == 0) {
      data = 100;
    }

    std::vector<int> pathToDest;
    sendHyperCube(&data, 1, MPI_INT, 0, 1, 0, MPI_COMM_WORLD, &pathToDest);

    std::vector<int> correctPathToDest = {0, 1};
    if (ProcRank == 1) {
      ASSERT_EQ(data, 100);
    }
    if (ProcRank == 0) {
      ASSERT_EQ(pathToDest, correctPathToDest);
    }
  }
}

TEST(Hypercube_MPI, Test_Hypercube_Send_From_0_To_3) {
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if (ProcNum >= 4) {
    int data;
    if (ProcRank == 0) {
      data = 100;
    }

    std::vector<int> pathToDest;
    sendHyperCube(&data, 1, MPI_INT, 0, 3, 0, MPI_COMM_WORLD,
                  &pathToDest);

    std::vector<int> correctPathToDest = {0, 1, 3};
    if (ProcRank == 3) {
      ASSERT_EQ(data, 100);
    }
    if (ProcRank == 0) {
      ASSERT_EQ(pathToDest, correctPathToDest);
    }
  }
}

TEST(Hypercube_MPI, Test_Hypercube_Send_From_1_To_6) {
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if (ProcNum >= 8) {
    int data;
    if (ProcRank == 1) {
      data = 100;
    }

    std::vector<int> pathToDest;
    sendHyperCube(&data, 1, MPI_INT, 1, 6, 0, MPI_COMM_WORLD,
                  &pathToDest);

    std::vector<int> correctPathToDest = {1, 0, 2, 6};
    if (ProcRank == 6) {
      ASSERT_EQ(data, 100);
    }
    if (ProcRank == 0) {
      ASSERT_EQ(pathToDest, correctPathToDest);
    }
  }
}

TEST(Hypercube_MPI, Test_Hypercube_Send_Big_Data) {
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  if (ProcNum >= 2) {
    int *data = new int[1000000];
    int *correctData = getRandomVector(1000000, 42);

    if (ProcRank == 0) {
      for (int i = 0; i < 1000000; i++) {
        data[i] = correctData[i];
      }
    }

    std::vector<int> pathToDest;
    sendHyperCube(data, 1000000, MPI_INT, 0, 1, 0, MPI_COMM_WORLD,
                  &pathToDest);

    if (ProcRank == 1) {
      for (int i = 0; i < 1000000; i++) {
        ASSERT_EQ(data[i], correctData[i]);
      }
    }
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
