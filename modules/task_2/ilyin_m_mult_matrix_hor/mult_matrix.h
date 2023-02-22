// Copyright 2023 Ilyin Maxim
#ifndef MODULES_TASK_2_ILYIN_M_MULT_MATRIX_HOR_MULT_MATRIX_H_
#define MODULES_TASK_2_ILYIN_M_MULT_MATRIX_HOR_MULT_MATRIX_H_

#include <vector>
#include <string>

std::vector<int> getRandomMatrix(int n, int m);
void printMatrix(std::vector<int> vec);
std::vector<int> getMultMatrixParallel(const std::vector<int>& pMatrix1,
                                const std::vector<int>& pMatrix2, int n, int m);
std::vector<int> getMultMatrixSequential(const std::vector<int>& pMatrix1,
                                const std::vector<int>& pMatrix2, int n, int m);
std::vector<int> getMultMatrixSequential2(const std::vector<int>& pMatrix1,
                                const std::vector<int>& pMatrix2, int n, int m, int start, int finish);
std::vector<int> transposeMatrix(const std::vector<int> &matrix, int n, int m);
#endif  // MODULES_TASK_2_ILYIN_M_MULT_MATRIX_HOR_MULT_MATRIX_H_

