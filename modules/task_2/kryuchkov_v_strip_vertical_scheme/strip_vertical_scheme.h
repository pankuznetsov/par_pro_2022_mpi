// Copyright 2022 Kryuchkov Vladimir
#ifndef MODULES_TASK_2_KRYUCHKOV_V_STRIP_VERTICAL_SCHEME_STRIP_VERTICAL_SCHEME_H_
#define MODULES_TASK_2_KRYUCHKOV_V_STRIP_VERTICAL_SCHEME_STRIP_VERTICAL_SCHEME_H_

int* init_empty_matrix(int rows);
int* get_random_matrix(int rows, int columns);
void sequential_matrix_multiplication(const int* A, const int A_rows,
                                      const int A_columns, const int* B,
                                      const int B_rows, int* res);
void parallel_matrix_multiplication(const int* A, const int A_rows,
                                    const int A_columns, const int* B,
                                    const int B_rows, int* res);

#endif  // MODULES_TASK_2_KRYUCHKOV_V_STRIP_VERTICAL_SCHEME_STRIP_VERTICAL_SCHEME_H_
