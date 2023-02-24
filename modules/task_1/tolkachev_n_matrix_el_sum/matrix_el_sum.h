// Copyright 2022 Tolkachev Nikita
#ifndef MODULES_TASK_1_TOLKACHEV_N_MATRIX_EL_SUM_MATRIX_EL_SUM_H_
#define MODULES_TASK_1_TOLKACHEV_N_MATRIX_EL_SUM_MATRIX_EL_SUM_H_

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>

int getParallelSum(std::vector<int> global_matrix);

int getVectorSum(std::vector<int> vec);

std::vector<int> getRandomMatrix(int m, int n);

#endif  // MODULES_TASK_1_TOLKACHEV_N_MATRIX_EL_SUM_MATRIX_EL_SUM_H_
