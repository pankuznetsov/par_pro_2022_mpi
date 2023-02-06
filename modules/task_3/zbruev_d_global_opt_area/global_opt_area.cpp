// Copyright 2022 Zbruev Denis
#include <mpi.h>
#include <random>
#include <algorithm>
#include <cmath>
#include "../../../modules/task_3/zbruev_d_global_opt_area/global_opt_area.h"

constexpr double epsilon = 1e-5;
constexpr double minus_inf = -1e+305;

void calc_probability(double* result, double m, int i, 
    const std::vector<double>& vec, std::function<double(double)> func) {
    double mdiff = (vec[i] - vec[i - 1]) * m;
    double fdiff = func(vec[i]) - func(vec[i - 1]);
    *result = mdiff + (fdiff * fdiff) / mdiff - 2 * (func(vec[i]) + func(vec[i - 1]));
}

void glob_optimization_sequen(double* result, double start, double end, std::function<double(double)> func) {
    constexpr int iter = 1000;
    constexpr double r = 2.0;  // reliability for constant m

    std::vector<double> r_vec(iter + 1, minus_inf);

    double M = abs((func(r_vec[1]) - func(r_vec[0])) / (r_vec[1] - r_vec[0]));
    double m = M == 0 ? 1 : M * r;

    r_vec[0] = start;
    r_vec[1] = end;
    r_vec[2] = (r_vec[1] + r_vec[0]) / 2 - (func(r_vec[1]) - func(r_vec[0])) / (2 * m);

    int index = 1;
    for (int c = 2; c < iter; c++) {
        sort(r_vec.begin(), r_vec.begin() + c + 1);

        // Lipschitz constant M
        M = abs((func(r_vec[1]) - func(r_vec[0])) / (r_vec[1] - r_vec[0]));
        for (int i = 1; i <= c; i++) {
            M = std::max(M, abs((func(r_vec[i]) - func(r_vec[i - 1])) / (r_vec[i] - r_vec[i - 1])));
        }

        // Lipschitz constant m
        m = M == 0 ? 1 : M * r;

        // Probability
        double p;
        calc_probability(&p, m, 1, r_vec, func);
        index = 1;
        for (int i = 1; i <= c; i++) {
            double cp;
            calc_probability(&cp, m, i, r_vec, func);
            if (p < cp) {
                p = cp;
                index = i;
            }
        }

        r_vec[c + 1] = (r_vec[index] + r_vec[index - 1]) / 2 - (func(r_vec[index]) - func(r_vec[index - 1])) / (2 * m);
        if (r_vec[index] - r_vec[index - 1] < epsilon) {
            break;
        }
    }
    *result = r_vec[index];
}

void glob_optimization_parallel(double* result, double start, double end, std::function<double(double)> func) {
    constexpr int iter = 64;
    constexpr double r = 2.0;

    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size == 1) {
        return glob_optimization_sequen(result, start, end, func);
    }

    double loc_res;
    std::vector<double> all_loc_res(size);
    std::vector<double> r_vec(iter + 1, minus_inf);

    double M = abs((func(r_vec[1]) - func(r_vec[0])) / (r_vec[1] - r_vec[0]));
    double m = M == 0 ? 1 : M * r;

    r_vec[0] = start;
    r_vec[1] = end;
    r_vec[2] = (r_vec[1] + r_vec[0]) / 2 - (func(r_vec[1]) - func(r_vec[0])) / (2 * m);

    int index = 1, c = 2;
    for (; c < iter; c++) {
        sort(r_vec.begin(), r_vec.begin() + c + 1);

        // Lipschitz constant M
        M = abs((func(r_vec[1]) - func(r_vec[0])) / (r_vec[1] - r_vec[0]));
        for (int i = 1; i <= c; i++) {
            M = std::max(M, abs((func(r_vec[i]) - func(r_vec[i - 1])) / (r_vec[i] - r_vec[i - 1])));
        }

        // Lipschitz constant m
        m = M == 0 ? 1 : M * r;

        // Probability
        double p;
        calc_probability(&p, m, 1, r_vec, func);
        index = 1;
        for (int i = 1; i <= c; i++) {
            double cp;
            calc_probability(&cp, m, i, r_vec, func);
            if (p < cp) {
                p = cp;
                index = i;
            }
        }

        r_vec[c + 1] = (r_vec[index] + r_vec[index - 1]) / 2 - (func(r_vec[index]) - func(r_vec[index - 1])) / (2 * m);
        if (r_vec[index] - r_vec[index - 1] < epsilon) {
            break;
        }
    }
    sort(r_vec.begin(), r_vec.begin() + c + 1);

    double l_start = r_vec[rank * iter / size];
    double l_end = size - rank == 1 ? r_vec[iter - 1] : r_vec[(rank + 1) * iter / size];
    glob_optimization_sequen(&loc_res, l_start, l_end, func);

    MPI_Gather(&loc_res, 1, MPI_DOUBLE, &all_loc_res[0], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        double min = all_loc_res[0];
        for (int i = 0; i < size; i++) {
            if (func(all_loc_res[i]) < func(min)) {
                min = all_loc_res[i];
            }
        }
        *result = min;
    }
}
