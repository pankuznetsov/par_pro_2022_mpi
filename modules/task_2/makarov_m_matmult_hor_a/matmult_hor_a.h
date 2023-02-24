// Copyright 2023 Makarov Mikhail
#ifndef MODULES_TASK_2_MAKAROV_M_MATMULT_HOR_A_MATMULT_HOR_A_H_
#define MODULES_TASK_2_MAKAROV_M_MATMULT_HOR_A_MATMULT_HOR_A_H_

#include <vector>
#include <string>

#define MY_ROOT (0)

std::vector<int> getRandVector(int vecSize);
void printAsMatrix(const std::vector<int>& vec, int m, int n);

std::vector<int> multSequential(const std::vector<int>& matrixA,
                              const std::vector<int>& matrixB, int m1, int n1,
                              int m2, int n2);
std::vector<int> multParallel(const std::vector<int>& matrixA,
                              const std::vector<int>& matrixB, int m1, int n1,
                              int m2, int n2);

#endif  // MODULES_TASK_2_MAKAROV_M_MATMULT_HOR_A_MATMULT_HOR_A_H_
