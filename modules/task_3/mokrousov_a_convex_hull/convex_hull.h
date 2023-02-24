// Copyright 2022 Mokrousov Artem
#ifndef MODULES_TASK_3_MOKROUSOV_A_CONVEX_HULL_CONVEX_HULL_H_
#define MODULES_TASK_3_MOKROUSOV_A_CONVEX_HULL_CONVEX_HULL_H_

#include <stack>

double** getRandomMas(int count);

bool SravnenieMore(int num, double* _mid, double* fi, double* r);
bool SravnenieLess(int num, double* _mid, double* fi, double* r);

double** QuickSort(double** mas, int first, int last, double* fi, double* r);

double det(double* p1, double* p2, double* p3, int s);
std::stack<double*> ConvSort(double** mas, const int count);

#endif  // MODULES_TASK_3_MOKROUSOV_A_CONVEX_HULL_CONVEX_HULL_H_
