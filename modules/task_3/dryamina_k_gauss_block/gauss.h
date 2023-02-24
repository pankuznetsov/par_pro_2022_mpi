// Copyright 2022 Dryamina Kseniya
#ifndef MODULES_TASK_3_DRYAMINA_K_GAUSS_BLOCK_GAUSS_H_
#define MODULES_TASK_3_DRYAMINA_K_GAUSS_BLOCK_GAUSS_H_

#include <vector>

const int gaussianRadius = 1;
const int gaussianSize = 3;
const int gaussianKernel[] = {
    1, 2, 1,
    2, 4, 2,
    1, 2, 1
};
const int gaussianNorm = 16;

std::vector<double> getRandomImage(int rows, int cols);
std::vector<double> getSequentialOperations(std::vector<double> image, int rows, int cols);
std::vector<double> getParallelOperations(std::vector<double> global_image, int rows, int cols);

#endif  // MODULES_TASK_3_DRYAMINA_K_GAUSS_BLOCK_GAUSS_H_
