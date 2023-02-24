// Copyright 2022 Tolkachev Nikita
#ifndef MODULES_TASK_3_TOLKACHEV_N_RADIX_BATCHER_INT_RADIX_BATCHER_INT_H_
#define MODULES_TASK_3_TOLKACHEV_N_RADIX_BATCHER_INT_RADIX_BATCHER_INT_H_

#include <stdio.h>
#include <mpi.h>
#include <random>
#include <iostream>
#include <vector>
using std::vector;
using std::swap;
vector<int> RandomVec(int n);
void compexch(int* ap, int* bp);
vector<int> merge(vector<vector<int>> v);
void BatchMerge(vector<int>* arr, int n, int left, int right);
void intSort(vector<int>* arr);
void intSortCat(vector<int>* arr, int cat);
int MaxCategoryNumber(int num);
vector<int> ParSort(vector<int> arr, int size);
#endif  // MODULES_TASK_3_TOLKACHEV_N_RADIX_BATCHER_INT_RADIX_BATCHER_INT_H_
