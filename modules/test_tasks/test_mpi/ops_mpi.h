// Copyright 2022 Lapin Dmitriy
#ifndef MODULES_TASK_1_LAPIN_D_MATRIX_MINIMUM_OPS_MPI_H_
#define MODULES_TASK_1_LAPIN_D_MATRIX_MINIMUM_OPS_MPI_H_
#include <vector>
#include <string>

std::vector<std::vector<int>> CreateMatrix(int m, int n);
std::vector<int> getVectorTransponMt(const std::vector<std::vector<int>>& matrix);
int getParallelOperations(std::vector<std::vector<int>> matrix);
int getSequentialOperations(std::vector<std::vector<int>> matrix);



#endif  
