// Copyright 2022 Tolkachev Nikita
#ifndef MODULES_TASK_2_TOLKACHEV_N_SIMPLE_ITER_MET_SIMPLE_ITER_MET_H_
#define MODULES_TASK_2_TOLKACHEV_N_SIMPLE_ITER_MET_SIMPLE_ITER_MET_H_

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>

std::vector<double> simple_iter_met_parallel(const std::vector<double>& A,
    const std::vector<double>& b,
    int n, int max_iter_count,
    double epsilon);

std::vector<double> get_random_vector(int n);

std::vector<double> matrix_vector_prod(const std::vector<double>& A,
    const std::vector<double>& x, int n);

std::vector<double> simple_iter_met_sequential(const std::vector<double>& A,
    const std::vector<double>& b,
    int n, int max_iter_count,
    double epsilon);

std::vector<double> get_random_matrix_diagonal_dominance(int n);

#endif  // MODULES_TASK_2_TOLKACHEV_N_SIMPLE_ITER_MET_SIMPLE_ITER_MET_H_
