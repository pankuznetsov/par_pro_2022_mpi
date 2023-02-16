#ifndef MODULES_TASK_3_LEBEDINSKIY_I_QUICKSORT_H
#define MODULES_TASK_3_LEBEDINSKIY_I_QUICKSORT_H

#include <random>
#include <vector>
#include <utility>

bool check(int* arr, int n);
std::pair<int, int> split(int* arr, int n);
void merge(int* arr1, int size1, int size2);
void single_quick_sort(int* arr, int n);
void parallel_quick_sort(int* arr, size_t n);

#endif // MODULES_TASK_3_LEBEDINSKIY_I_QUICKSORT_H