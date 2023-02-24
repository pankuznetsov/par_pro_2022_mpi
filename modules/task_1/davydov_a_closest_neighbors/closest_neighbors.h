// Copyright 2022 Davydov Andrey
#ifndef MODULES_TASK_1_DAVYDOV_A_CLOSEST_NEIGHBORS_CLOSEST_NEIGHBORS_H_
#define MODULES_TASK_1_DAVYDOV_A_CLOSEST_NEIGHBORS_CLOSEST_NEIGHBORS_H_

#include <vector>

std::vector<int> getRandomVector(int size);

int closestNeighborsSequential(std::vector<int> vec);
int closestNeighborsParallel(std::vector<int> vec, const int vecSize);

#endif  // MODULES_TASK_1_DAVYDOV_A_CLOSEST_NEIGHBORS_CLOSEST_NEIGHBORS_H_
