// Copyright 2022 Reshetnik Yana
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/reshetnik_y_max_in_matrix_row/max_in_matrix_row.h"

int* get_random_matrix(int x_size, int y_size) {
    std::random_device ran_dev;
    std::mt19937 gener(ran_dev());
    int* m = new int[x_size * y_size];
    for (int i = 0; i < x_size * y_size; i++) {
        m[i] = gener() % 10000;
    }
    return m;
}

int* max_by_row(int* m, int x_size, int y_size) {
    int* res = new int[y_size];
    for (int y = 0; y < y_size; y++) {
        res[y] = m[y * x_size];
        for (int x = 1; x < x_size; x++) {
            int i = x + y * x_size;
            if (m[i] > res[y]) {
                res[y] = m[i];
            }
        }
    }
    return res;
}

int* parallel_max_by_row(int* m, int x_size, int y_size) {
    int len = x_size * y_size;

    if (len == 0) {
        return nullptr;
    }

    int* local = new int[len];

    int proc_count;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int seg = y_size / proc_count;
    if (y_size % proc_count) {
        seg++;
    }

    if (rank != 0) {
        MPI_Bcast(local, len, MPI_INT, 0, MPI_COMM_WORLD);
    } else {
        for (int i = 0; i < len; i++) {
            local[i] = m[i];
        }
        MPI_Bcast(local, len, MPI_INT, 0, MPI_COMM_WORLD);
    }

    int last_row = seg * (rank + 1);
    int loc_len = seg;

    if (last_row > y_size) {
        last_row = y_size;
        loc_len -= y_size - last_row;
    }

    int* loc_max = max_by_row(local + rank * seg * x_size, x_size, loc_len);

    if (rank != 0) {
        MPI_Gather(loc_max, loc_len, MPI_INT, 0, 0, 0, 0, MPI_COMM_WORLD);
        return nullptr;
    } else {
        int* result = new int[len];
        MPI_Gather(loc_max, loc_len, MPI_INT, result, seg, MPI_INT, 0, MPI_COMM_WORLD);
        return result;
    }

    delete[] loc_max;
    delete[] local;
}
