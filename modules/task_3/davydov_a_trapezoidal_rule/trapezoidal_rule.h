// Copyright 2022 Davydov Andrey
#ifndef MODULES_TASK_3_DAVYDOV_A_TRAPEZOIDAL_RULE_TRAPEZOIDAL_RULE_H_
#define MODULES_TASK_3_DAVYDOV_A_TRAPEZOIDAL_RULE_TRAPEZOIDAL_RULE_H_

double plus(double x, double y);
double minus(double x, double y);
double multiple(double x, double y);
double division(double x, double y);
double quadSum(double x, double y);

double sequentialTrapezoidalRule(double xa, double xb, double ya, double yb, int n, double (*func)(double, double));
double parallelTrapezoidalRule(double xa, double xb, double ya, double yb, int n, double (*func)(double, double));

#endif  // MODULES_TASK_3_DAVYDOV_A_TRAPEZOIDAL_RULE_TRAPEZOIDAL_RULE_H_
