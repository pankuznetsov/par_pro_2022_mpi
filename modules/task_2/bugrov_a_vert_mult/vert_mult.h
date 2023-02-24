// Copyright 2022 Bugrov Andrey

#ifndef MODULES_TASK_2_BUGROV_A_VERT_MULT_VERT_MULT_H_
#define MODULES_TASK_2_BUGROV_A_VERT_MULT_VERT_MULT_H_

void RandCreator(int* matrix_or_vector, int size);
void SeqMult(int* matrix, int* vector, int* result_vector, int n, int m);
void ParMult(int* matrix, int* vector, int* result_vector, int n, int m);
#endif  // MODULES_TASK_2_BUGROV_A_VERT_MULT_VERT_MULT_H_
