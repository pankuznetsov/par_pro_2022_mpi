// Copyright 2023 Alexander Terentiev
#ifndef MODULES_TASK_2_TERENTIEV_A_VERT_MATR_VECT_MULT_VERT_MATR_VECT_MULT_H_
#define MODULES_TASK_2_TERENTIEV_A_VERT_MATR_VECT_MULT_VERT_MATR_VECT_MULT_H_

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <random>
#include <vector>

using std::cout;
using std::vector;

const int maxValue = 1e3;

class Matrix {
 private:
    vector<int> data;
    int str;
    int col;

 public:
    Matrix(int n, int m);

    void Randomize();

    int Str() const;
    int Col() const;

    int Get(int i, int j) const;
    void Set(int i, int j, int value);

    void Print();
};

vector<int> MultipleSequential(const Matrix &matr, const vector<int> &vect);

vector<int> MultipleParallel(const Matrix &matr, const vector<int> &vect);

vector<int> RandomVector(int size);

void PrintVector(vector<int> vect);

#endif  // MODULES_TASK_2_TERENTIEV_A_VERT_MATR_VECT_MULT_VERT_MATR_VECT_MULT_H_
