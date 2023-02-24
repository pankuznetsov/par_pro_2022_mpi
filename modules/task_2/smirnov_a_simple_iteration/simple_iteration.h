// Copyright 2018 Nesterov Alexander
#ifndef MODULES_TASK_2_SMIRNOV_A_SIMPLE_ITERATION_SIMPLE_ITERATION_H_
#define MODULES_TASK_2_SMIRNOV_A_SIMPLE_ITERATION_SIMPLE_ITERATION_H_

#include <vector>
#include <string>

void getRandMatrVect(int size, int leftBorder, int rightBorder,
  std::vector<double>* A, std::vector<double>* b);

double checkSolution(int size, const std::vector<double>& A,
  const std::vector<double>& b, const std::vector<double>& X);

void getAlfaBetaMatrix(const std::vector<double>& A, const std::vector<double>& b,
  std::vector<double>* alfaMatrix,
  std::vector<double>* betaMatrix, int size);

std::vector<double> parSimpleIteration(const std::vector<double>& A,
  const std::vector<double>& b, int _size, double eps = 0.001, int _countIterations = 20,
  bool checkIterations = false);

double getAccuracy(std::vector<double> prevX, std::vector<double> curX);

std::vector<double> seqSimpleIteration(const std::vector<double>& A,
  const std::vector<double>& b, double eps = 0.001, int _countIterations = 20,
  bool checkIterations = false);

#endif  // MODULES_TASK_2_SMIRNOV_A_SIMPLE_ITERATION_SIMPLE_ITERATION_H_
