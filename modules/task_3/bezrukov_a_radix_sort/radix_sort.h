// Copyright 2023 Bezrukov Aleksandr
#ifndef MODULES_TASK_3_BEZRUKOV_A_RADIX_SORT_RADIX_SORT_H_
#define MODULES_TASK_3_BEZRUKOV_A_RADIX_SORT_RADIX_SORT_H_
#include <vector>

std::vector<double> genRandomVector(int len);
void vecPrint(const std::vector<double>& targetVec);
void radixSortSequential(std::vector<double>* in);
void radixSortParallel(std::vector<double>* globVec, int glob_vec_size);

#endif  // MODULES_TASK_3_BEZRUKOV_A_RADIX_SORT_RADIX_SORT_H_
