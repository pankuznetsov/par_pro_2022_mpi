// Copyright 2022 me
#ifndef MODULES_TASK_3_KROLEVETS_N_SHELL_MERGE_SORT_SHELL_MERGE_SORT_H_
#define MODULES_TASK_3_KROLEVETS_N_SHELL_MERGE_SORT_SHELL_MERGE_SORT_H_

#include <mpi.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <random>
#include <vector>

uint64_t next_power_of_2(uint64_t);
void shell_sort(float*, uint64_t);
void merge(float*, uint64_t);
std::vector<float> generate_array(uint64_t);
std::vector<float> merge_sort_par(const std::vector<float>&);
bool check(const std::vector<float>&);

#endif  // MODULES_TASK_3_KROLEVETS_N_SHELL_MERGE_SORT_SHELL_MERGE_SORT_H_
