// Copyright 2022 Zbruev Denis
#ifndef MODULES_TASK_3_ZBRUEV_D_GLOBAL_OPT_AREA_GLOBAL_OPT_AREA_H_
#define MODULES_TASK_3_ZBRUEV_D_GLOBAL_OPT_AREA_GLOBAL_OPT_AREA_H_
#include <functional>
#include <vector>
#include <string>

void calc_probability(double* result, double m, int i, 
	const std::vector<double>& vec, std::function<double(double)> func);
void glob_optimization_sequen(double* result, double start, double end, std::function<double(double)> func);
void glob_optimization_parallel(double* result, double start, double end, std::function<double(double)> func);

#endif  // MODULES_TASK_3_ZBRUEV_D_GLOBAL_OPT_AREA_GLOBAL_OPT_AREA_H_
