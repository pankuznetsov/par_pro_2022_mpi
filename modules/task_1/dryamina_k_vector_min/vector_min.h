// Copyright 2022 Dryamina Kseniya
#ifndef MODULES_TASK_1_DRYAMINA_K_VECTOR_MIN_VECTOR_MIN_H_
#define MODULES_TASK_1_DRYAMINA_K_VECTOR_MIN_VECTOR_MIN_H_

#include <vector>

std::vector<int> getRandomVector(int size);

int vectorMinSequential(std::vector<int> vec);

int vectorMinParallel(std::vector<int> vec, const int vecSize);

#endif  // MODULES_TASK_1_DRYAMINA_K_VECTOR_MIN_VECTOR_MIN_H_
