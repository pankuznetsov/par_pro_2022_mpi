// Copyright 2022 Davydov Andrey
#ifndef MODULES_TASK_2_DAVYDOV_A_JACOBI_JACOBI_H_
#define MODULES_TASK_2_DAVYDOV_A_JACOBI_JACOBI_H_

#include <vector>

std::vector<float> getRandomVector(int n);
std::vector<float> getRandomMatrix(int n);

std::vector<float> getX0(const std::vector<float> &A, const std::vector<float> &b, const int n);
std::vector<float> jacobiSequential(const std::vector<float> &A, const std::vector<float> &b,
                                    const std::vector<float> &X0, const float eps, const int n);
std::vector<float> jacobiParallel(const std::vector<float> &A, const std::vector<float> &b,
                                    const std::vector<float> &X0, const float eps, const int n);

#endif  // MODULES_TASK_2_DAVYDOV_A_JACOBI_JACOBI_H_
