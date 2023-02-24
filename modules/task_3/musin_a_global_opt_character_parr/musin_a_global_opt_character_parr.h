// Copyright 2022 Musin Alexandr
#ifndef MODULES_TASK_3_MUSIN_A_GLOBAL_OPT_CHARACTER_PARR_MUSIN_A_GLOBAL_OPT_CHARACTER_PARR_H_
#define MODULES_TASK_3_MUSIN_A_GLOBAL_OPT_CHARACTER_PARR_MUSIN_A_GLOBAL_OPT_CHARACTER_PARR_H_
#include <functional>

double* twoDecOptimization(double flag, double exitParamter, double ay,
                          double by, double ax, double bx,
                          std::function<double(double, double)> foo);
void optimization(double flag, double exitParamter, double a, double b,
                  std::function<double(double, double)> foo);

#endif  // MODULES_TASK_3_MUSIN_A_GLOBAL_OPT_CHARACTER_PARR_MUSIN_A_GLOBAL_OPT_CHARACTER_PARR_H_
