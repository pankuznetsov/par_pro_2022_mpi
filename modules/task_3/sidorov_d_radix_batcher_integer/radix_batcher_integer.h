// Copyright 2022 Sidorov Denis
#ifndef MODULES_TASK_3_SIDOROV_D_RADIX_BATCHER_INTEGER_RADIX_BATCHER_INT_H_
#define MODULES_TASK_3_SIDOROV_D_RADIX_BATCHER_INTEGER_RADIX_BATCHER_INT_H_

#include <mpi.h>
#include <stdio.h>

#include <iostream>
#include <random>
#include <vector>
using std::swap;
using std::vector;
vector<int> RandomVec(int n);
void compexch(int* ap, int* bp);
vector<int> merge(vector<vector<int>> v);
void BatchMerge(vector<int>* arr, int n, int left, int right);
void intSort(vector<int>* arr);
void intSortCat(vector<int>* arr, int cat);
int MaxCategoryNumber(int num);
vector<int> ParSort(vector<int> arr, int size);
#endif  // MODULES_TASK_3_SIDOROV_D_RADIX_BATCHER_INTEGER_RADIX_BATCHER_INT_H_
