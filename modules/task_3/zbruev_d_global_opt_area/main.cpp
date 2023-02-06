// Copyright 2022 Zbruev Denis
#include <gtest/gtest.h>
#include <vector>
#include "./global_opt_area.h"
#include <gtest-mpi-listener.hpp>

double simple_func(double x) {
    return x;
}

TEST(GlobalOptimizationArea, SimpleTest) {
    constexpr double eps = 0.1;
    int rank;
    double res;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    glob_optimization_sequen(&res, -1, 1, simple_func);

    if (rank == 0) {
        ASSERT_TRUE(res - eps < -1.0 && -1.0 < res + eps);
    }
}

double polin_cos(double x) {
    return x + x * x + cos(3 * x);
}

TEST(GlobalOptimizationArea, TestSeqCorrectness) {
    constexpr double eps = 0.1;
    int rank;
    double res;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    glob_optimization_sequen(&res, -2, 1, polin_cos);

    if (rank == 0) {
        ASSERT_TRUE(res - eps < -0.95 && -0.95 < res + eps);
    }
}

TEST(GlobalOptimizationArea, PolinCosTest) {
    constexpr double eps = 0.1;
    int rank;
    double s_res, p_res;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    glob_optimization_sequen(&s_res, -2, 1, polin_cos);
    glob_optimization_parallel(&p_res, -2, 1, polin_cos);

    if (rank == 0) {
        ASSERT_TRUE(s_res - eps < p_res && p_res < s_res + eps);
    }
}

double sin_cos(double x) {
    return sin(x) + cos(x);
}

TEST(GlobalOptimizationArea, SinCosTest) {
    constexpr double eps = 0.1;
    int rank;
    double s_res, p_res;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    glob_optimization_sequen(&s_res, -5, 0, sin_cos);
    glob_optimization_parallel(&p_res, -5, 0, sin_cos);

    if (rank == 0) {
        ASSERT_TRUE(s_res - eps < p_res && p_res < s_res + eps);
    }
}

double polin(double x) {
    return -x + x * x - x * x * x + x * x * x * x;
}

TEST(GlobalOptimizationArea, PolinTest) {
    constexpr double eps = 0.1;
    int rank;
    double s_res, p_res;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    glob_optimization_sequen(&s_res, -3, 1, polin);
    glob_optimization_parallel(&p_res, -3, 1, polin);

    if (rank == 0) {
        ASSERT_TRUE(s_res - eps < p_res&& p_res < s_res + eps);
    }
}

double polin_sin(double x) {
    return  -3 * x + x * x - 15 * sin(x);
}

TEST(GlobalOptimizationArea, PolinSinTest) {
    constexpr double eps = 0.1;
    int rank;
    double s_res, p_res;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    glob_optimization_sequen(&s_res, 4, 10, polin_sin);
    glob_optimization_parallel(&p_res, 4, 10, polin_sin);

    if (rank == 0) {
        ASSERT_TRUE(s_res - eps < p_res&& p_res < s_res + eps);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
