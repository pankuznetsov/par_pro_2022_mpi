// Copyright 2023 Pankov Anatoliy
#include "../../../modules/task_1/pankov_a_mat_rows_mins/mat_rows_mins.h"

#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// Generates random vector
std::vector<int> random_vec_gen(int n) {
    std::random_device device;
    std::mt19937 engine(device());
    std::vector<int> vec;
    for (int i = 0; i < n; i++) {
        int new_elem = engine() % 10000;
        vec.push_back(new_elem);
    }
    return vec;
}

// Executes algorithm sequential
std::vector<int> sequential_mat_rows_mins(int m, int n,
                                          const std::vector<int>& mat) {
    std::vector<int> res;

    // Iteration per rows
    for (int i = 0; i < m; i++) {
        int i_col_min = mat[i * n];
        // Iteration per cols to find i_col_min
        for (int j = 0; j < n; j++) {
            if (mat[i * n + j] < i_col_min) {
                i_col_min = mat[i * n + j];
            }
        }
        res.push_back(i_col_min);
    }

    return res;
}

// Executes algorithm parallel
std::vector<int> parallel_mat_rows_mins(int m, int n,
                                        const std::vector<int>& mat) {
    int cur_process_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &cur_process_rank);
    int all_processes_count;
    MPI_Comm_size(MPI_COMM_WORLD, &all_processes_count);

    const int active_processes_count = std::min(m, all_processes_count);
    const bool root_process = (cur_process_rank == 0);
    const bool other_active =
        (cur_process_rank != 0) && cur_process_rank < active_processes_count;
    const int rows_per_process_usual = m / active_processes_count;
    const int rows_additional = m % active_processes_count;

    if (!root_process && !other_active) return std::vector<int>(0);

    if (root_process) {
        for (int r_active = 1; r_active < active_processes_count; r_active++) {
            MPI_Send(mat.data() +
                         n * (rows_per_process_usual + rows_additional) +
                         (r_active - 1) * n * rows_per_process_usual,
                     n * rows_per_process_usual, MPI_INT, r_active, 0,
                     MPI_COMM_WORLD);
        }
    }

    std::vector<int> rows_to_process(rows_per_process_usual * n);
    std::vector<int> local_part_of_the_result;
    std::vector<int> global_result(m);

    if (root_process) {
        rows_to_process = std::vector<int>(
            mat.begin(),
            mat.begin() + (rows_per_process_usual + rows_additional) * n);

        local_part_of_the_result = sequential_mat_rows_mins(
            rows_per_process_usual + rows_additional, n, rows_to_process);

        for (int i = 0; i < local_part_of_the_result.size(); i++)
            global_result[i] = local_part_of_the_result[i];

        for (int i = 1; i < active_processes_count; i++) {
            MPI_Status status;
            MPI_Recv(
                global_result.data() + rows_additional + rows_per_process_usual * i,
                rows_per_process_usual, MPI_INT, i, i, MPI_COMM_WORLD, &status);
        }

    } else if (other_active) {
        MPI_Status status;
        MPI_Recv(rows_to_process.data(), rows_per_process_usual * n, MPI_INT, 0,
                 0, MPI_COMM_WORLD, &status);

        local_part_of_the_result = sequential_mat_rows_mins(
            rows_per_process_usual, n, rows_to_process);

        MPI_Send(local_part_of_the_result.data(), rows_per_process_usual,
                 MPI_INT, 0, cur_process_rank, MPI_COMM_WORLD);
    }

    return global_result;
}
