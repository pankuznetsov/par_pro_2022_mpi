// Copyright 2023 Anoschenkova Anna
#ifndef MODULES_TASK_1_ANOSCHENKOVA_A_MAT_COLS_MIN_MAT_COLS_MIN_H_
#define MODULES_TASK_1_ANOSCHENKOVA_A_MAT_COLS_MIN_MAT_COLS_MIN_H_

#include <vector>

// Help functions
std::vector<int> randColMat(int m, int n);
void vecShow(const std::vector<int>& vec);

// Algorithms
std::vector<int> minByColsSequential(int m, int n,
                                     const std::vector<int>& mat);
std::vector<int> minByColsParallel(int m, int n,
                                    const std::vector<int>& mat);

#endif  // MODULES_TASK_1_ANOSCHENKOVA_A_MAT_COLS_MIN_MAT_COLS_MIN_H_
