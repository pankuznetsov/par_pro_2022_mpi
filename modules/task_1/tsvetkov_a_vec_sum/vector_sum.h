// Copyright 2022 Tsvetkov
#ifndef MODULES_TASK_1_TSVETKOV_A_VEC_SUM_VECTOR_SUM_H_
#define MODULES_TASK_1_TSVETKOV_A_VEC_SUM_VECTOR_SUM_H_

#include <string>
#include <vector>

std::vector<int> getRandomVector(int size);

void printVector(const std::vector<int>& vec);

int getSumSequential(std::vector<int> vec);

int getSumParallel(std::vector<int> globalVector);

#endif  // MODULES_TASK_1_TSVETKOV_A_VEC_SUM_VECTOR_SUM_H_
