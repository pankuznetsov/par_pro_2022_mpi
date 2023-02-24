// Copyright 2022 Musin Alexandr
#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <gtest-mpi-listener.hpp>

#include "./musin_a_global_opt_character_parr.h"

TEST(Parallel_Operations_MPI, Test_But_Function_Y) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double ax = -3;
    double bx = 5;
    double ay = -4;
    double by = 5;
    double* res;
    double exitParameter = 0.1;
    std::function<double(double, double)> foo = [](double x, double y) {
        return (pow(x + 2 * y - 7, 2) + pow(2 * x + y - 5, 2));
    };
    if (rank == 0) {
        res = twoDecOptimization(-100, exitParameter, ay, by, ax, bx, foo);
    } else {
        optimization(-100, exitParameter, ax, bx, foo);
    }

    if (rank == 0) {
        ASSERT_NEAR(res[1], 3, 2 * exitParameter);
    }
}

TEST(Parallel_Operations_MPI, Test_powX_2_plus_powY_2_100_100_X) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double ax = -5;
    double bx = 1;
    double ay = -10;
    double by = 10;
    double* res;
    double exitParameter = 0.05;
    std::function<double(double, double)> foo = [](double x, double y) {
        return (pow(x + 1, 2) + pow(y + 5, 2));
    };
    if (rank == 0) {
        res = twoDecOptimization(-100, exitParameter, ay, by, ax, bx, foo);
    } else {
        optimization(-100, exitParameter, ax, bx, foo);
    }

    if (rank == 0) {
        ASSERT_NEAR(res[0], -1, 2 * exitParameter);
    }
}

TEST(Parallel_Operations_MPI, Test_powX_2_plus_powY_2_100_100_Y) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double ax = -5;
    double bx = 1;
    double ay = -7;
    double by = 3;
    double* res;
    double exitParameter = 0.05;
    std::function<double(double, double)> foo = [](double x, double y) {
        return (pow(x + 1, 2) + pow(y + 5, 2));
    };
    if (rank == 0) {
        res = twoDecOptimization(-100, exitParameter, ay, by, ax, bx, foo);
    } else {
        optimization(-100, exitParameter, ax, bx, foo);
    }

    if (rank == 0) {
        ASSERT_NEAR(res[1], -5, 2 * exitParameter);
    }
}

TEST(Parallel_Operations_MPI, Test_Y_Sinx_Plus_SinY) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double ax = -1;
    double bx = 1;
    double ay = -1;
    double by = 1;
    double* res;
    double exitParameter = 0.01;
    std::function<double(double, double)> foo = [](double x, double y) {
        return -(cos(x) + cos(y));
    };
    if (rank == 0) {
        res = twoDecOptimization(-100, exitParameter, ay, by, ax, bx, foo);
    } else {
        optimization(-100, exitParameter, ax, bx, foo);
    }

    if (rank == 0) {
        ASSERT_NEAR(res[1], 0, 2 * exitParameter);
    }
}

TEST(Parallel_Operations_MPI, Test_X_Sinx_Plus_SinY) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double ax = -1;
    double bx = 1;
    double ay = -1;
    double by = 1;
    double* res;
    double exitParameter = 0.01;
    std::function<double(double, double)> foo = [](double x, double y) {
        return -(cos(x) + cos(y));
    };
    if (rank == 0) {
        res = twoDecOptimization(-100, exitParameter, ay, by, ax, bx, foo);
    } else {
        optimization(-100, exitParameter, ax, bx, foo);
    }

    if (rank == 0) {
        ASSERT_NEAR(res[0], 0, 2 * exitParameter);
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
