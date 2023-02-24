// Copyright 2022 Tuzhilkina Polina
#ifndef MODULES_TASK_2_TUZHILKINA_P_STAR_TOP_STAR_TOP_H_
#define MODULES_TASK_2_TUZHILKINA_P_STAR_TOP_STAR_TOP_H_

#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <functional>

std::vector<int> getRandomVec(int size);

void StarSend(void* buf, int count, MPI_Datatype datatype, int from, int dest,
              int tag, MPI_Comm comm);

void printVecElements(const std::vector<int>& vec);

#endif  // MODULES_TASK_2_TUZHILKINA_P_STAR_TOP_STAR_TOP_H_
