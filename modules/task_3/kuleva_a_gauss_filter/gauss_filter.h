// Copyright 2023 Kuleva Anna
#ifndef MODULES_TASK_3_KULEVA_A_GAUSS_FILTER_GAUSS_FILTER_H_
#define MODULES_TASK_3_KULEVA_A_GAUSS_FILTER_GAUSS_FILTER_H_

#include <vector>

using Matrix = std::vector<unsigned char>;
using CoreMatrix = std::vector<std::vector<double>>;

Matrix generateImage(int sizeX, int sizeY);
int translateCoordinates(int x, int y, int sizeX);
unsigned char getGauseFilterForPixel(const Matrix& matrix, int x, int y, int sizeX);
Matrix applyGauseFilterPar(const Matrix& image, int sizeX, int sizeY);

#endif  // MODULES_TASK_3_KULEVA_A_GAUSS_FILTER_GAUSS_FILTER_H_
