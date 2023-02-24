// Copyright 2022 Sharovatov Daniil
#include "../../../modules/task_1/sharovatov_d_scalar_product/scalar_product.h"

std::vector<int> getRandomVector(int size) {
  if (size <= 0) {
    throw(-1);
  }

  std::vector<int> randomVector(size);

  std::random_device dev;
  std::mt19937 gen(dev());

  for (int i = 0; i < size; i++) {
    randomVector[i] = gen() % 100;
  }

  return randomVector;
}

int getScalarProductSequential(std::vector<int> v1, std::vector<int> v2) {
  if (v1.size() <= 0 || v2.size() <= 0) {
    throw(-1);
  }
  if (v1.size() != v2.size()) {
    throw(-1);
  }

  int sum = 0;

  for (int i = 0; i < v1.size(); i++) {
    sum += v1[i] * v2[i];
  }

  return sum;
}

int getScalarProductParallel(std::vector<int> v1, std::vector<int> v2) {
  if (v1.size() <= 0 || v2.size() <= 0) {
    throw(-1);
  }
  if (v1.size() != v2.size()) {
    throw(-1);
  }

  int ProcNum, ProcRank;

  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int sum = 0, allSum = 0;
  int size = v1.size();

  int remainder = size % ProcNum;
  size = size / ProcNum;

  if (ProcRank == 0) {
    for (int i = 1; i < ProcNum - 1; i++) {
      MPI_Send(&v1[i * size], size, MPI_INT, i, 0, MPI_COMM_WORLD);
      MPI_Send(&v2[i * size], size, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
    if (ProcNum > 1) {
      MPI_Send(&v1[(ProcNum - 1) * size], size + remainder, MPI_INT,
               ProcNum - 1, 0, MPI_COMM_WORLD);
      MPI_Send(&v2[(ProcNum - 1) * size], size + remainder, MPI_INT,
               ProcNum - 1, 0, MPI_COMM_WORLD);
    }
  }

  std::vector<int> recieveV1(size);
  std::vector<int> recieveV2(size);
  if (ProcRank == 0) {
    recieveV1 = std::vector<int>(v1.begin(), v1.begin() + size);
    recieveV2 = std::vector<int>(v2.begin(), v2.begin() + size);
  } else if (ProcRank == ProcNum - 1) {
    MPI_Status status;
    recieveV1.resize(size + remainder);
    recieveV2.resize(size + remainder);
    MPI_Recv(&recieveV1[0], size + remainder, MPI_INT, 0, 0, MPI_COMM_WORLD,
             &status);
    MPI_Recv(&recieveV2[0], size + remainder, MPI_INT, 0, 0, MPI_COMM_WORLD,
             &status);
  } else {
    MPI_Status status;
    MPI_Recv(&recieveV1[0], size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&recieveV2[0], size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  for (int i = 0; i < recieveV1.size(); i++) {
    sum += recieveV1[i] * recieveV2[i];
  }

  MPI_Reduce(&sum, &allSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return allSum;
}
