// Copyright 2022 Yarakhtin Anton
#ifndef MODULES_TASK_3_YARAKHTIN_A_CANNONS_ALGORITHM_CANNONS_ALGORITHM_H_
#define MODULES_TASK_3_YARAKHTIN_A_CANNONS_ALGORITHM_CANNONS_ALGORITHM_H_

#include <mpi.h>
#include <vector>
#include <string>
#include <cmath>

class Matrix {
 public:
    Matrix(int x_size, int y_size);
    Matrix(const Matrix& m);
    ~Matrix();

    Matrix& operator=(const Matrix& m);

    int x_size, y_size;
    double* data = nullptr;
};

Matrix generateMatrix(int x_size, int y_size);
Matrix multiplySequentially(const Matrix& A, const Matrix& B);
Matrix sumSequentially(const Matrix& A, const Matrix& B);
Matrix multiplyParallel(const Matrix& A, const Matrix& B);

#endif  // MODULES_TASK_3_YARAKHTIN_A_CANNONS_ALGORITHM_CANNONS_ALGORITHM_H_
