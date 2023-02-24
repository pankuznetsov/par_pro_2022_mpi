// Copyright 2018 Nesterov Alexander
#ifndef MODULES_TASK_3_SMIRNOV_A_SPARSE_MATRIX_MULT_SPARSE_MATRIX_MULT_H_
#define MODULES_TASK_3_SMIRNOV_A_SPARSE_MATRIX_MULT_SPARSE_MATRIX_MULT_H_

#include <vector>
#include <string>
class SparseMatrix {
 public:
  std::vector<double> values;
  std::vector<int> columnIndex;
  std::vector<int> rowIndex;
  int countRows;
  int countColumns;

  explicit SparseMatrix(std::vector<std::vector<double>> matrix);

  SparseMatrix Transpose();
  SparseMatrix() = default;
  void PrintStats();

  bool operator ==(SparseMatrix m) const;
};

SparseMatrix seqSparseMatrixMult(const std::vector<std::vector<double>>& _A,
  const std::vector<std::vector<double>>& _B);
SparseMatrix parSparseMatrixMult(const std::vector<std::vector<double>>& _A,
  const std::vector<std::vector<double>>& _B);

#endif  // MODULES_TASK_3_SMIRNOV_A_SPARSE_MATRIX_MULT_SPARSE_MATRIX_MULT_H_
