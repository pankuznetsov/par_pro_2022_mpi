// Copyright 2022 Sharovatov Daniil
#ifndef MODULES_TASK_3_SHAROVATOV_D_SOBEL_OPERATOR_SOBEL_OPERATOR_H_
#define MODULES_TASK_3_SHAROVATOV_D_SOBEL_OPERATOR_SOBEL_OPERATOR_H_

#include <mpi.h>;

#include <random>
#include <vector>
#include <algorithm>

std::vector<std::vector<int>> getRandomImage(int width, int height);

std::vector<int> matrixToVector(std::vector<std::vector<int>> imageMatrix);

std::vector<std::vector<int>> vectorToMatrix(std::vector<int> imageVector,
                                             int height);

int clamp(int value, int min, int max);

int calculateNewPixelColor(std::vector<std::vector<int>> image, int x, int y);

std::vector<std::vector<int>> sobelOperatorSequential(std::vector<std::vector<int>> image);

std::vector<std::vector<int>> sobelOperatorParallel(std::vector<std::vector<int>> image);

#endif  // MODULES_TASK_3_SHAROVATOV_D_SOBEL_OPERATOR_SOBEL_OPERATOR_H_
