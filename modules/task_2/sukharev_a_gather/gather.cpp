// Copyright 2022 Sukharev Artem

#include "../../../modules/task_2/sukharev_a_gather/gather.h"

#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <cstring>

std::vector<int> getRandomVectorInt(const int size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<int> randomVector;
  randomVector.resize(size);
  for (int i = 0; i < size; i++) {
    randomVector[i] = gen();
  }
  return randomVector;
}
std::vector<float> getRandomVectorFloat(const int size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<float> randomVector;
  randomVector.resize(size);
  for (int i = 0; i < size; i++) {
    randomVector[i] = static_cast<float>(gen());
  }
  return randomVector;
}
std::vector<double> getRandomVectorDouble(const int size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<double> randomVector;
  randomVector.resize(size);
  for (int i = 0; i < size; i++) {
    randomVector[i] = static_cast<double>(gen());
  }
  return randomVector;
}

void printVector(const std::vector<int>& tmpVector) {
  for (auto x : tmpVector) std::cout << x << " ";
  std::cout << "\n";
}

int MY_Gather(void* sendbuf, int sendcount, MPI_Datatype sendtype,
              void* recvbuf, int recvcount, MPI_Datatype recvtype, int root,
              MPI_Comm comm) {
  int rank, procNum;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  recvcount *= procNum;
  if (root > procNum) return MPI_ERR_ROOT;
  if (sendtype != recvtype) return MPI_ERR_ARG;
  if (rank == root && recvcount != procNum * sendcount) return MPI_ERR_ARG;
  if (sendbuf == nullptr || root == rank && recvbuf == nullptr)
    return MPI_ERR_BUFFER;

  char* tmp = nullptr;
  int sizeType = 0;
  MPI_Type_size(sendtype, &sizeType);
  int messageSize = sendcount * sizeType;
  int firstBit = 0;
  if (rank != 0) {
    firstBit = rank - (rank & (rank - 1));
  } else {
    firstBit = procNum;
  }
  int sizetmpbuf = 0;
  if (rank == 0)
    sizetmpbuf = recvcount * sizeType;
  else
    sizetmpbuf = std::min(procNum - rank, firstBit) * messageSize;

  tmp = new char[sizetmpbuf];
  memcpy(tmp, sendbuf, messageSize);
  for (int i = 1; i < firstBit && rank + i < procNum; i *= 2) {
    MPI_Status status;
    int tmpSendCount = std::min(i, procNum - (rank + i)) * sendcount;
    MPI_Recv(tmp + i * messageSize, tmpSendCount, sendtype, rank + i, 0, comm,
             &status);
  }
  if (rank != 0) {
    MPI_Send(tmp, sizetmpbuf / sizeType, sendtype, rank - firstBit, 0, comm);
  }

  if (root == 0) {
    if (rank == 0) {
      memcpy(recvbuf, tmp, sizetmpbuf);
    }
  } else {
    if (rank == 0) {
      MPI_Send(tmp, sizetmpbuf / sizeType, sendtype, root, 1, comm);
    }
    if (rank == root) {
      MPI_Status status;
      MPI_Recv(recvbuf, recvcount, sendtype, 0, 1, comm, &status);
    }
  }

  delete tmp;
  return MPI_SUCCESS;
}
