// Copyright 2022 Krainev Alexander
#ifndef MODULES_TASK_1_KRAINEV_A_MONTE_CARLO_MONTE_CARLO_H_
#define MODULES_TASK_1_KRAINEV_A_MONTE_CARLO_MONTE_CARLO_H_

double funcPlus(double x);
double funcMinus(double x);
double funcSqrt(double x);
double funcPow(double x);

double monteCarloSequential(int n, double a, double b, double h, double (*func)(double));
double monteCarloParallel(int n, double a, double b, double h, double (*func)(double));

#endif  // MODULES_TASK_1_KRAINEV_A_MONTE_CARLO_MONTE_CARLO_H_
