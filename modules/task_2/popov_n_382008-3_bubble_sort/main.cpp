// Copyright 2023 Popov Nikita

#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./bubble_sort.h"
#include <ctime>


TEST(ParallelBubbleSort, TestOne) {
  int rankOfThread;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankOfThread);

  int arrSize = 3;
  int* arr = new int[arrSize];

  for (int i = 0; i < arrSize; i++) {
    arr[i] = ArrGenerate(arrSize);
  }

  int* parrallelRArr = ParallelBubbleSort(arr, arrSize);

  if (rankOfThread == 0) {

    BubbleSort(arr, arrSize);
    ASSERT_EQ(parrallelRArr, arr);
  }
}


TEST(ParallelBubbleSort, TestTwo) {
  int rankOfThread;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankOfThread);

  int arrSize = 6;
  int* arr = new int[arrSize];

  for (int i = 0; i < arrSize; i++) {
    arr[i] = ArrGenerate(arrSize);
  }

  int* parrallelRArr = ParallelBubbleSort(arr, arrSize);

  if (rankOfThread == 0) {
    BubbleSort(arr, arrSize);
    ASSERT_EQ(parrallelRArr, arr);
  }
}

TEST(ParallelBubbleSort, TestThree) {
  int rankOfThread;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankOfThread);

  int arrSize = 9;
  int* arr = new int[arrSize];

  for (int i = 0; i < arrSize; i++) {
    arr[i] = ArrGenerate(arrSize);
  }

  int* parrallelRArr = ParallelBubbleSort(arr, arrSize);

  if (rankOfThread == 0) {
    BubbleSort(arr, arrSize);
    ASSERT_EQ(parrallelRArr, arr);
  }
}

TEST(ParallelBubbleSort, TestFour) {
  int rankOfThread;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankOfThread);

  int arrSize = 12;
  int* arr = new int[arrSize];

  for (int i = 0; i < arrSize; i++) {
    arr[i] = ArrGenerate(arrSize);
  }

  int* parrallelRArr = ParallelBubbleSort(arr, arrSize);

  if (rankOfThread == 0) {
    BubbleSort(arr, arrSize);
    ASSERT_EQ(parrallelRArr, arr);
  }
}

TEST(ParallelBubbleSort, TestFive) {
  int rankOfThread;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankOfThread);

  int arrSize = 15;
  int* arr = new int[arrSize];

  for (int i = 0; i < arrSize; i++) {
    arr[i] = ArrGenerate(arrSize);
  }

  int* parrallelRArr = ParallelBubbleSort(arr, arrSize);

  if (rankOfThread == 0) {
    BubbleSort(arr, arrSize);
    ASSERT_EQ(parrallelRArr, arr);
  }
}

TEST(ParallelBubbleSort, TestSix) {
  int rankOfThread;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankOfThread);

  int arrSize = 18;
  int* arr = new int[arrSize];

  for (int i = 0; i < arrSize; i++) {
    arr[i] = ArrGenerate(arrSize);
  }

  int* parrallelRArr = ParallelBubbleSort(arr, arrSize);

  if (rankOfThread == 0) {
    BubbleSort(arr, arrSize);
    ASSERT_EQ(parrallelRArr, arr);
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
