// Copyright 2023 Raschetnov Alexei
#ifndef MODULES_TASK_2_RASCHETNOV_A_RING_RING_H_
#define MODULES_TASK_2_RASCHETNOV_A_RING_RING_H_

#include <mpi.h>
#include <vector>

int SendRingParallel(int information, int count, MPI_Datatype datatype,
                     int root, int destination, int delta, int tag, MPI_Comm comm);
int getResult(int information, int size, int delta);
int getRandomNumber(int leftborder, int rightborder);
std::vector<int> constructPath(int root, int destination, int size);

#endif  // MODULES_TASK_2_RASCHETNOV_A_RING_RING_H_
