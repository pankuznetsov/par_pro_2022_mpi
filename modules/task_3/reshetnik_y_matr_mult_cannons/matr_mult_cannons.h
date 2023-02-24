// Copyright 2022 Reshetnik Yana
#ifndef MODULES_TASK_3_RESHETNIK_Y_MATR_MULT_CANNONS_MATR_MULT_CANNONS_H_
#define MODULES_TASK_3_RESHETNIK_Y_MATR_MULT_CANNONS_MATR_MULT_CANNONS_H_

void fill_matrix(double* m, int columns, int rows);
void multiply_matrix(double* result, double* m1, int columns1, int rows1, double* m2, int columns2, int rows2);
void cannon_parallel_multiply(double* result, double* m1, int columns1, int rows1, double* m2, int columns2, int rows2);

#endif  // MODULES_TASK_3_RESHETNIK_Y_MATR_MULT_CANNONS_MATR_MULT_CANNONS_H_
