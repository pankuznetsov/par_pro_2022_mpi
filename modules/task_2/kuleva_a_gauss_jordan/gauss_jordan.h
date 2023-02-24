// Copyright 2023 Kuleva Anna
#ifndef MODULES_TASK_2_KULEVA_A_GAUSS_JORDAN_GAUSS_JORDAN_H_
#define MODULES_TASK_2_KULEVA_A_GAUSS_JORDAN_GAUSS_JORDAN_H_

#include <vector>

using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;

Vector getGauseJordanPar(const Matrix& A, const Vector& b);

#endif  // MODULES_TASK_2_KULEVA_A_GAUSS_JORDAN_GAUSS_JORDAN_H_
