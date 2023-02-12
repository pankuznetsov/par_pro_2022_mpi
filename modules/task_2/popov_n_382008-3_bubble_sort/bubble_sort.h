// Copyright 2023 Popov Nikita

#ifndef MODULES_TASK_2_POPOV_N_382008_3_BUBBLE_SORT_MPI_H_
#define MODULES_TASK_2_POPOV_N_382008_3_BUBBLE_SORT_MPI_H_

int ArrGenerate(int arrSize);
void BubbleSort(int *arr, int size);
int *ParallelBubbleSort(int *inputArr, int arrSize);

#endif //MODULES_TASK_2_POPOV_N_382008_3_BUBBLE_SORT_MPI_H_
