// Copyright 2023 Bezrukov Aleksandr
#ifndef MODULES_TASK_3_BEZRUKOV_A_RADIX_SORT_RADIX_SORT_H_
#define MODULES_TASK_3_BEZRUKOV_A_RADIX_SORT_RADIX_SORT_H_
#include <vector>

std::vector<double> genRandomVector(int len);
void printVector(const std::vector<double>& vec);

int leftOfThePoint(double number);
int rightOfThePoint(double number);
int getDigit(double number, int radix);
std::vector<double> sortByOneRadix(const std::vector<double>& vect, int rad);

void radixSortSequential(std::vector<double>* array);
void radixSortParallel(std::vector<double>* globVec, int glob_vec_size);

#endif  // MODULES_TASK_3_BEZRUKOV_A_RADIX_SORT_RADIX_SORT_H_
