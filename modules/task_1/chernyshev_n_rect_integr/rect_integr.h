// Copyright 2022 Chernyshev Nikita
#ifndef MODULES_TASK_1_CHERNYSHEV_N_RECT_INTEGR_RECT_INTEGR_H_
#define MODULES_TASK_1_CHERNYSHEV_N_RECT_INTEGR_RECT_INTEGR_H_

#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
using std::cout;

double *FuncValues_Sin(double from, double to, int val_quantity,
                       double *step_size);

double *FuncValues_X2(double from, double to, int val_quantity,
                      double *step_size);

double TrueIntegral_Sin(double from, double to);

double TrueIntegral_X2(double from, double to);

double IntegralParallel(double *func_values, double step_size,
                        int val_quantity);

#endif  // MODULES_TASK_1_CHERNYSHEV_N_RECT_INTEGR_RECT_INTEGR_H_
