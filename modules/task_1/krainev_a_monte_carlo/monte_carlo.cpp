// Copyright 2022 Krainev Alexander
#include <mpi.h>
#include <cmath>
#include <random>
#include "../../../modules/task_1/krainev_a_monte_carlo/monte_carlo.h"

double funcPlus(double x) { return x + 13.; }
double funcMinus(double x) { return x - 13.; }
double funcSqrt(double x) { return std::sqrt(x) + 13.; }
double funcPow(double x) { return std::pow(x, 3) + 13.; }

double monteCarloSequential(int n, double a, double b, double h, double (*func)(double)) {
    std::random_device device;
    std::mt19937 gen(device());

    std::uniform_real_distribution<> xRealDistribution(a, b);
    std::uniform_real_distribution<> yRealDistribution(0., h);

    int count = 0;

    for (int i = 0; i < n; ++i) {
        if (yRealDistribution(gen) < func(xRealDistribution(gen))) {
            ++count;
        }
    }

    return (static_cast<double>(count) / static_cast<double>(n)) * (b - a) * h;
}

double monteCarloParallel(int n, double a, double b, double h, double (*func)(double)) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::random_device device;
    std::mt19937 gen(device());

    std::uniform_real_distribution<> xRealDistribution(a, b);
    std::uniform_real_distribution<> yRealDistribution(0., h);

    int count = 0;

    const int delta = n / size;

    for (int i = 0; i < delta; ++i) {
        if (yRealDistribution(gen) < func(xRealDistribution(gen))) {
            ++count;
        }
    }

    int globalCount;
    MPI_Reduce(&count, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        return (static_cast<double>(globalCount) / static_cast<double>(n)) * (b - a) * h;
    }

    return 0;
}
