// Copyright 2022 Grachev Valentin
#ifndef MODULES_TASK_1_GRACHEV_V_MATRIX_MAX_MATRIX_MAX_H_
#define MODULES_TASK_1_GRACHEV_V_MATRIX_MAX_MATRIX_MAX_H_

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

#define NEG_INF -10000000;

class TMatrix {
 private:
    double *arr;
    int str_count;
    int col_count;

 public:
    TMatrix(int m, int n);
    TMatrix(const TMatrix& other);
    TMatrix& operator=(const TMatrix& other);
    ~TMatrix();

    void SetElement(int i, int j, double value);
    double GetElement(int i, int j) const;
    void FillRandom(int min, int max);
    int GetStringCount() const;
    int GetColumnCount() const;
    double *GetStringValues(int str_number) const;
    double *GetColumnValues(int col_number) const;
    void PrintElements() const;
};

double GetMax(double *arr, int size);

void GetMatrixMaxParallel(const TMatrix &matrix, double *time, double *result);

double GetMatrixMaxLinear(const TMatrix &matrix, double *time);

#endif  // MODULES_TASK_1_GRACHEV_V_MATRIX_MAX_MATRIX_MAX_H_
