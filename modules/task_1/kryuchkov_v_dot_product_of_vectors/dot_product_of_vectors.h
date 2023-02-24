// Copyright 2022 Kryuchkov Vladimir
#ifndef MODULES_TASK_1_KRYUCHKOV_V_DOT_PRODUCT_OF_VECTORS_DOT_PRODUCT_OF_VECTORS_H_
#define MODULES_TASK_1_KRYUCHKOV_V_DOT_PRODUCT_OF_VECTORS_DOT_PRODUCT_OF_VECTORS_H_
#include <vector>

int get_sequential_product(const std::vector<int>& a,
                           const std::vector<int>& b);

int get_sequential_product_std(const std::vector<int>& a,
                               const std::vector<int>& b);

int get_parallel_product(std::vector<int> a, std::vector<int> b);

std::vector<int> get_random_vector(int size);

#endif  // MODULES_TASK_1_KRYUCHKOV_V_DOT_PRODUCT_OF_VECTORS_DOT_PRODUCT_OF_VECTORS_H_
