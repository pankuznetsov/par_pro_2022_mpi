// Copyright 2023 Anoschenkova Anna
#ifndef MODULES_TASK_2_ANOSCHENKOVA_A_CONTRAST_INCR_CONTRAST_INCR_H_
#define MODULES_TASK_2_ANOSCHENKOVA_A_CONTRAST_INCR_CONTRAST_INCR_H_
#include <vector>
std::vector<int> genImage(int rows, int cols);

std::vector<int> incrContrast(const std::vector<int>& a, int rows, int cols,
                              int contrast);
std::vector<int> incrContrastParallel(const std::vector<int>& a, int rows,
                                      int cols, int contrast);
#endif  // MODULES_TASK_2_ANOSCHENKOVA_A_CONTRAST_INCR_CONTRAST_INCR_H_
