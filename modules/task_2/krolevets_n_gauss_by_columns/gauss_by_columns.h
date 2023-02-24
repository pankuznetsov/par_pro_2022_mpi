// Copyright 2022 me
#ifndef MODULES_TASK_2_KROLEVETS_N_GAUSS_BY_COLUMNS_GAUSS_BY_COLUMNS_H_
#define MODULES_TASK_2_KROLEVETS_N_GAUSS_BY_COLUMNS_GAUSS_BY_COLUMNS_H_

#include <mpi.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <random>
#include <type_traits>
#include <utility>
#include <vector>

class Gaussian_Solver {
 public:
  Gaussian_Solver(uint32_t x, uint32_t y) : size_x(x), size_y(y) {}
  void generate_matrix();
  std::vector<float> solve_parallel();
  std::vector<float> solve_sequential();
  float check_answer(const std::vector<float>& ans);  // returns max delta

 private:
  float& get(uint32_t x, uint32_t y) { return matrix[y + size_y * x]; }
  float get_orig(uint32_t x, uint32_t y) { return matrix[y + size_y * x]; }
  void permutate(uint32_t);
  uint32_t size_x;
  uint32_t size_y;
  std::vector<float> matrix;       // stored by columns
  std::vector<float> orig_matrix;  // stored by columns
};

#endif  // MODULES_TASK_2_KROLEVETS_N_GAUSS_BY_COLUMNS_GAUSS_BY_COLUMNS_H_
