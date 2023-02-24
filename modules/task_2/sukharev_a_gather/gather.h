// Copyright 2022 Sukharev Artem

#ifndef MODULES_TASK_2_SUKHAREV_A_GATHER_GATHER_H_
#define MODULES_TASK_2_SUKHAREV_A_GATHER_GATHER_H_

#include <mpi.h>

#include <vector>

void printVector(const std::vector<int>& tmpVector);

std::vector<int> getRandomVectorInt(const int size);
std::vector<float> getRandomVectorFloat(const int size);
std::vector<double> getRandomVectorDouble(const int size);

int MY_Gather(void* sendbuf, int sendcount, MPI_Datatype sendtype,
              void* recvbuf, int recvcount, MPI_Datatype recvtype, int root,
              MPI_Comm comm);

#endif  // MODULES_TASK_2_SUKHAREV_A_GATHER_GATHER_H_
