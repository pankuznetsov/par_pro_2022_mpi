// Copyright 2023 Raschetnov Alexei
#ifndef MODULES_TASK_1_RASCHETNOV_A_ROWS_SUM_ROWS_SUM_H_
#define MODULES_TASK_1_RASCHETNOV_A_ROWS_SUM_ROWS_SUM_H_

#include <vector>
#include <string>

std::vector<int> getSequentialSum(const std::vector<int>& local_matrix, int rows, int columns);
std::vector<int> getParallelSum(const std::vector<int>& global_matrix, int rows, int columns);
std::vector<int> getRandomMatrix(int rows, int columns);

#endif  // MODULES_TASK_1_RASCHETNOV_A_ROWS_SUM_ROWS_SUM_H_
