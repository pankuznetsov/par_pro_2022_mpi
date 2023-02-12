// Copyright 2023 Popov Nikita

#include "../../../modules/task_2/popov_n_382008-3_bubble_sort/bubble_sort.h"
#include <mpi.h>
#include <random>
#include <ctime>


int ArrGenerate(int arrSize) {
  std::mt19937 generator;
  generator.seed(static_cast<unsigned int>(time(0)));
  return static_cast<int>(generator() % 500);
}

void BubbleSort(int *arr, int size) {
  for (int i = 0; i < size - 1; i++) {
    for (int j = 0; j < size - i - 1; j++) {
      if (arr[j] > arr[j + 1]) std::swap(arr[j], arr[j + 1]);
    }
  }
}

int *ParallelBubbleSort(int *arr, int aSize) {
  int size = aSize;

  int numOfThreads;
  MPI_Comm_size(MPI_COMM_WORLD, &numOfThreads);

  int rankOfThread;
  MPI_Comm_rank(MPI_COMM_WORLD, &rankOfThread);

  std::srand(time(nullptr));

  int blockSize = size / numOfThreads;
  int *subArr = new int[blockSize];

  MPI_Scatter(arr, blockSize, MPI_INT, subArr, blockSize, MPI_INT, 0,
              MPI_COMM_WORLD);
  BubbleSort(subArr, blockSize);
  MPI_Gather(subArr, blockSize, MPI_INT, arr, blockSize, MPI_INT, 0,
             MPI_COMM_WORLD);

  if (rankOfThread == 0) BubbleSort(arr, size);
  return arr;
}
