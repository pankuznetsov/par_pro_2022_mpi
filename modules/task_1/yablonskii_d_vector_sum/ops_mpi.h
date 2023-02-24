// Copyright 2023 Yablonskii Dmitriy
#ifndef MODULES_TASK_1_YABLONSKII_D_VECTOR_SUM_OPS_MPI_H_
#define MODULES_TASK_1_YABLONSKII_D_VECTOR_SUM_OPS_MPI_H_

#include <vector>
#include <string>

std::vector<int> rand_vec(int size);

int stand_summa(std::vector<int> V);
int par_summa(std::vector<int> Vector, int size);

#endif  // MODULES_TASK_1_YABLONSKII_D_VECTOR_SUM_OPS_MPI_H_
