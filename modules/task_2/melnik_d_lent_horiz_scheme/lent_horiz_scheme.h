// Copyright 2023 Melnik Denis
#ifndef MODULES_TASK_2_MELNIK_D_LENT_HORIZ_SCHEME_LENT_HORIZ_SCHEME_H_
#define MODULES_TASK_2_MELNIK_D_LENT_HORIZ_SCHEME_LENT_HORIZ_SCHEME_H_

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <random>
#include <vector>

using std::cout;
using std::vector;

const int maxInt = 100000;

class Matrix {
 private:
    vector<int> matrix;
    int width;
    int height;

 public:
    Matrix(int h, int w);

    int W() const { return width; }
    int H() const { return height; }

    void SetValue(int i, int j, int value) { matrix[i * width + j] = value; }
    int GetValue(int i, int j) const { return matrix[i * width + j]; }

    static Matrix ToMatrix(int *arr, int h, int w);
    static Matrix GetRandom(int h, int w);
};

vector<int> Sequence(const Matrix &originMatrix,
                     const vector<int> &originVector);

vector<int> Parallel(const Matrix &originMatrix,
                     const vector<int> &originVector);

vector<int> GetRandomVector(int size);

#endif  // MODULES_TASK_2_MELNIK_D_LENT_HORIZ_SCHEME_LENT_HORIZ_SCHEME_H_
