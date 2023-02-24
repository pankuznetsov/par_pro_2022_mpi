// Copyright 2022 Tolkachev Nikita
#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_2/tolkachev_n_simple_iter_met/simple_iter_met.h"
std::vector<double> get_random_matrix_diagonal_dominance(int n) {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::vector<double> matrix(n * n);
    std::uniform_real_distribution<double> dist(0.0, 3.0);
    for (int i = 0; i < n; i++) {
        double sum_row = 0.0;
        for (int j = 0; j < n; j++) {
            matrix[i * n + j] = dist(gen);
            sum_row += matrix[i * n + j];
        }
        matrix[i * n + i] = sum_row * 3.0;
    }
    return matrix;
}
std::vector<double> matrix_vector_prod(const std::vector<double>& A,
    const std::vector<double>& x, int n) {
    std::vector<double> b(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) b[i] += A[i * n + j] * x[j];
    }
    return b;
}
std::vector<double> simple_iter_met_parallel(const std::vector<double>& A,
    const std::vector<double>& b,
    int n, int max_iter_count,
    double epsilon) {
    std::vector<double> x(n, 0);
    int comm_size;
    int rank;
    double local_diff;
    double max_diff;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int loc_size = n / comm_size;
    int rem = n % comm_size;
    std::vector<double> local_A(loc_size * n);
    std::vector<double> local_b(loc_size);
    std::vector<double> local_x(loc_size);
    if (rank == 0) {
        for (int i = 1; i < comm_size; i++) {
            MPI_Send(A.data() + (rem + i * loc_size) * n, loc_size * n,
                MPI_DOUBLE, i, 1,
                MPI_COMM_WORLD);
            MPI_Send(b.data() + rem + i * loc_size, loc_size, MPI_DOUBLE, i, 2,
                MPI_COMM_WORLD);
        }
        loc_size = loc_size + rem;
        local_A.resize(loc_size * n);
        local_b.resize(loc_size);
        local_x.resize(loc_size);
        local_A = std::vector<double>(A.begin(), A.begin() + loc_size * n);
        local_b = std::vector<double>(b.begin(), b.begin() + loc_size);
    } else {
        MPI_Recv(local_A.data(), loc_size * n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD,
            &status);
        MPI_Recv(local_b.data(), loc_size, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD,
            &status);
    }
    if (rank == 0) {
        for (int i = 0; i < loc_size; i++) {
            double diagonal_element = local_A[i * n + i];
            local_A[i * n + i ] = 0.0;
            local_b[i] = local_b[i] / diagonal_element;
            for (int j = 0; j < n; j++) {
                local_A[i * n + j] = local_A[i * n + j] / diagonal_element;
            }
        }
    } else {
        for (int i = 0; i < loc_size; i++) {
            double diagonal_element = local_A[i * n + i + rem +
                rank * loc_size];
            local_A[i * n + i + rem + rank * loc_size] = 0.0;
            local_b[i] = local_b[i] / diagonal_element;
            for (int j = 0; j < n; j++) {
                local_A[i * n + j] = local_A[i * n + j] / diagonal_element;
            }
        }
    }
    int counter = 0;
    do {
        for (int i = 0; i < loc_size; i++) {
            local_x[i] = 0.0;
            for (int j = 0; j < n; j++) {
                local_x[i] -= local_A[i * n + j] * x[j];
            }
            local_x[i] += local_b[i];
        }
        local_diff = 0.0;
        if (rank == 0) {
            for (int i = 0; i < loc_size; i++) {
                if (abs(local_x[i] - x[i]) > local_diff)
                    local_diff = abs(local_x[i] - x[i]);
            }
        } else {
            for (int i = 0; i < loc_size; i++) {
                if (abs(local_x[i] - x[i + rem + rank*loc_size]) > local_diff)
                    local_diff = abs(local_x[i] - x[i + rem + rank * loc_size]);
            }
        }
        MPI_Reduce(&local_diff, &max_diff, 1, MPI_DOUBLE, MPI_MAX, 0,
            MPI_COMM_WORLD);
        MPI_Bcast(&max_diff, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        if (max_diff > epsilon) {
            if (rank == 0) {
                std::copy(local_x.begin(), local_x.end(), x.begin());
                for (int i = 1; i < comm_size; i++) {
                    MPI_Recv(x.data() + rem + i * (loc_size-rem), loc_size-rem,
                        MPI_DOUBLE, i, 3,
                        MPI_COMM_WORLD, &status);
                }
            } else {
                MPI_Send(local_x.data(), loc_size,
                    MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
            }
            MPI_Bcast(x.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        }
        counter++;
    } while (max_diff > epsilon && counter < max_iter_count);
    return x;
}
std::vector<double> simple_iter_met_sequential(
    const std::vector<double>& A_matrix, const std::vector<double>& b_vec,
    int n, int max_iter_count, double epsilon) {
    std::vector<double> x(n, 0);
    std::vector<double> x_next(n);
    std::vector<double> A(n * n);
    std::vector<double> b(n);
    std::copy(A_matrix.begin(), A_matrix.end(), A.begin());
    std::copy(b_vec.begin(), b_vec.end(), b.begin());
    double diff;

    for (int i = 0; i < n; i++) {
        double diagonal_element = A[i * n + i];
        A[i * n + i] = 0.0;
        b[i] = b[i] / diagonal_element;
        for (int j = 0; j < n; j++) {
            A[i * n + j] = A[i * n + j] / diagonal_element;
        }
    }
    int counter = 0;
    do {
        for (int i = 0; i < n; i++) {
            x_next[i] = 0.0;
            for (int j = 0; j < n; j++) {
                x_next[i] -= A[i * n + j] * x[j];
            }
            x_next[i] += b[i];
        }
        diff = 0.0;
        for (int i = 0; i < n; i++) {
            if (abs(x_next[i] - x[i]) > diff) diff = abs(x_next[i] - x[i]);
        }
        if (diff > epsilon) std::copy(x_next.begin(), x_next.end(), x.begin());
        counter++;
    } while (diff > epsilon && counter < max_iter_count);
    return x;
}
std::vector<double> get_random_vector(int n) {
    std::vector<double> v(n);
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_real_distribution<double> dist(0.0, 3.0);
    for (int i = 0; i < n; ++i) v[i] = dist(gen);
    return v;
}
