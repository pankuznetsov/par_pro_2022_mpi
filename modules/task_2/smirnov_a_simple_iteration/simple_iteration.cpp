// Copyright 2018 Nesterov Alexander
#include <mpi.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include "../../../modules/task_2/smirnov_a_simple_iteration/simple_iteration.h"

void getRandMatrVect(int size, int leftBorder, int rightBorder,
  std::vector<double>* A, std::vector<double>* b) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_int_distribution<> dist(leftBorder, rightBorder);

  (*A).resize(size * size);
  (*b).resize(size);

  for (size_t i = 0; i < size; i++) {
    double sum = 0;
    for (size_t j = i * size; j < i * size + size; j++) {
      if (j == i + i * size) {
        continue;
      } else {
        (*A)[j] = dist(gen);
        sum += std::fabs((*A)[j]);
      }
    }
    (*b)[i] = dist(gen);
    (*A)[i + i * size] = sum + gen() % 17;
  }
}

double checkSolution(int size, const std::vector<double>& A,
  const std::vector<double>& b, const std::vector<double>& X) {
  std::vector<double> result(size, 0);
  for (size_t i = 0; i < size; i++) {
    for (size_t j = i * size; j < i * size + size; j++) {
      result[i] += A[j] * X[j - i * size];
    }
  }

  for (size_t i = 0; i < result.size(); i++) {
    result[i] = std::fabs(result[i] - b[i]);
  }

  return *std::max_element(result.begin(), result.end());
}

void getAlfaBetaMatrix(const std::vector<double>& A,
  const std::vector<double>& b,
  std::vector<double>* alfaMatrix,
  std::vector<double>* betaMatrix, int size) {
  for (size_t i = 0; i < size; i++) {
    for (size_t j = i * size; j < i * size + size; j++) {
      if (j == i + i * size) {
        if (A[j] == 0)
          throw std::string("Zero diagonal element");
        (*alfaMatrix)[j] = 0;
        (*betaMatrix)[i] = b[i] / A[j];
      } else {
        (*alfaMatrix)[j] = -(A[j] / A[i + i * size]);
      }
    }
  }
}

double getAccuracy(std::vector<double> prevX, std::vector<double> curX) {
  std::vector<double> delta(curX.size());
  for (size_t i = 0; i < curX.size(); i++) {
    delta[i] = fabs(curX[i] - prevX[i]);
  }
  return *std::max_element(delta.begin(), delta.end());
}

std::vector<double> seqSimpleIteration(const std::vector<double>& A,
  const std::vector<double>& b, double eps,
  int _countIterations, bool checkIterations) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int size = b.size();
  if (A.size() != size * size) {
    throw std::string("Not equal sizes");
  }
  int countIterations = 0;

  std::vector<double> prevX = b;
  std::vector<double> currentX = b;
  std::vector<double> alfaMatrix(size * size);
  std::vector<double> betaMatrix(size);
  getAlfaBetaMatrix(A, b, &alfaMatrix, &betaMatrix, size);

  do {
    countIterations++;
    prevX = currentX;
    for (size_t i = 0; i < size; i++) {
      double multRowByColumn = 0;
      for (size_t j = i * size; j < i * size + size; j++) {
        multRowByColumn += alfaMatrix[j] * prevX[j - i * size];
      }
      currentX[i] = betaMatrix[i] + multRowByColumn;
    }
  } while ((checkIterations && (countIterations < _countIterations)) ||
    (!checkIterations && getAccuracy(prevX, currentX) > eps));
  return currentX;
}

std::vector<double> parSimpleIteration(const std::vector<double>& A,
  const std::vector<double>& b, int _size, double eps,
  int _countIterations, bool checkIterations) {
  int countProc, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &countProc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  const int countRowsA = _size;
  const int lenMatrRow = countRowsA + 1;
  std::vector<double> AbMatrix;
  int countIterations = 0;

  if (rank == 0 && A.size() != countRowsA * countRowsA && b.size() != countRowsA) {
    throw std::string("Not equal sizes");
  }

  int dataPerProc = countRowsA / countProc;
  int remainder = countRowsA % countProc;

  if (dataPerProc == 0) {
    return seqSimpleIteration(A, b, eps, countIterations);
  }
  std::vector<double> prevX(countRowsA);
  std::vector<double> currentX(countRowsA);

  if (rank == 0) {
    std::vector<double> copyB = b;
    MPI_Bcast(copyB.data(), countRowsA, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  } else {
    MPI_Bcast(prevX.data(), countRowsA, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  std::vector<int> sendCounts(countProc);
  std::vector<int> displs(countProc);
  std::vector<int> recvCounts(countProc);
  std::vector<int> recvDispls(countProc);
  sendCounts[0] = (dataPerProc + remainder) * lenMatrRow;
  recvCounts[0] = dataPerProc + remainder;
  displs[0] = 0;
  recvDispls[0] = 0;

  for (int i = 1; i < countProc; i++) {
    sendCounts[i] = dataPerProc * lenMatrRow;
    recvCounts[i] = dataPerProc;
    displs[i] = (i * dataPerProc + remainder) * lenMatrRow;
    recvDispls[i] = i * dataPerProc + remainder;
  }

  std::vector<double> localAb(sendCounts[rank]);
  std::vector<double> localX(sendCounts[rank] / lenMatrRow);

  if (rank == 0) {
    AbMatrix.resize(A.size() + b.size());
    std::vector<double> alfaMatrix(countRowsA * countRowsA);
    std::vector<double> betaMatrix(countRowsA);
    getAlfaBetaMatrix(A, b, &alfaMatrix, &betaMatrix, countRowsA);

    for (size_t i = 0; i < countRowsA; i++) {
      for (size_t j = i * countRowsA; j < i * countRowsA + countRowsA; j++) {
        AbMatrix[j + i] = alfaMatrix[j];
      }
      AbMatrix[i * countRowsA + countRowsA + i] = betaMatrix[i];
    }
  }
  MPI_Scatterv(AbMatrix.data(), sendCounts.data(), displs.data(), MPI_DOUBLE,
    localAb.data(), sendCounts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

  do {
    countIterations++;
    prevX = currentX;
    for (size_t i = 0; i < localX.size(); i++) {
      double multRowByColumn = 0;
      for (size_t j = i * lenMatrRow; j < i * lenMatrRow + lenMatrRow - 1; j++) {
        multRowByColumn += localAb[j] * prevX[j - i * lenMatrRow];
      }
      localX[i] = localAb[i * countRowsA + countRowsA + i] + multRowByColumn;
    }
    MPI_Allgatherv(localX.data(), recvCounts[rank], MPI_DOUBLE,
      currentX.data(), recvCounts.data(), recvDispls.data(), MPI_DOUBLE, MPI_COMM_WORLD);
  } while ((checkIterations && (countIterations < _countIterations)) ||
    (!checkIterations && getAccuracy(prevX, currentX) > eps));
  return currentX;
}
