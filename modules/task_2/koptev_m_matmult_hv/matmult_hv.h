// Copyright 2023 Koptev Maksim
#ifndef MODULES_TASK_2_KOPTEV_M_MATMULT_HV_MATMULT_HV_H_
#define MODULES_TASK_2_KOPTEV_M_MATMULT_HV_MATMULT_HV_H_

#include <mpi.h>

#include <vector>

std::vector<int> getRandomMatrix(int matSize);
std::vector<int> getMatrixMultSeq(std::vector<int> matA,
                                         std::vector<int> matB,
                                         int matSize);
std::vector<int> getMatrixMultParellel(std::vector<int> matA,
                                                 std::vector<int> matB,
                                                 int matSize);

#endif  // MODULES_TASK_2_KOPTEV_M_MATMULT_HV_MATMULT_HV_H_
