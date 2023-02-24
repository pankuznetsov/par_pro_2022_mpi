// Copyright 2023 Pankov Anatoliy
#ifndef MODULES_TASK_1_PANKOV_A_MAT_ROWS_MINS_MAT_ROWS_MINS_H_
#define MODULES_TASK_1_PANKOV_A_MAT_ROWS_MINS_MAT_ROWS_MINS_H_
#include <vector>
std::vector<int> random_vec_gen(int n);
std::vector<int> sequential_mat_rows_mins(int m, int n,
                                      const std::vector<int>& matrix);
std::vector<int> parallel_mat_rows_mins(int m, int n,
                                    const std::vector<int>& matrix);
#endif  // MODULES_TASK_1_PANKOV_A_MAT_ROWS_MINS_MAT_ROWS_MINS_H_
