// Copyright 2022 Chernyshev Nikita
#include <gtest/gtest.h>
#include "../../../modules/task_1/chernyshev_n_rect_integr/rect_integr.h"
#include <gtest-mpi-listener.hpp>


TEST(Rectangle_Integral_Mpi, Test_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        double from = 0, to = 5;
        double true_res = TrueIntegral_Sin(from, to);
        double step, val_quantity = 10e2;
        double *values = FuncValues_Sin(from, to, val_quantity, &step);
        double paral_res = IntegralParallel(values, step, val_quantity);
        ASSERT_TRUE(abs(true_res - paral_res) < 10e-4);
    } else {
        IntegralParallel(nullptr, 0, 0);
    }
}

TEST(Rectangle_Integral_Mpi, Test_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        double from = -5, to = 10;
        double true_res = TrueIntegral_Sin(from, to);
        double step, val_quantity = 10e3;
        double *values = FuncValues_Sin(from, to, val_quantity, &step);
        double paral_res = IntegralParallel(values, step, val_quantity);
        ASSERT_TRUE(abs(true_res - paral_res) < 10e-5);
    } else {
        IntegralParallel(nullptr, 0, 0);
    }
}

TEST(Rectangle_Integral_Mpi, Test_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        double from = -5, to = 7;
        double true_res = TrueIntegral_X2(from, to);
        double step, val_quantity = 10e3;
        double *values = FuncValues_X2(from, to, val_quantity, &step);
        double paral_res = IntegralParallel(values, step, val_quantity);
        ASSERT_TRUE(abs(true_res - paral_res) < 10e-4);
    } else {
        IntegralParallel(nullptr, 0, 0);
    }
}

TEST(Rectangle_Integral_Mpi, Test_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        double from = -10, to = 12;
        double true_res = TrueIntegral_X2(from, to);
        double step, val_quantity = 10e4;
        double *values = FuncValues_X2(from, to, val_quantity, &step);
        double paral_res = IntegralParallel(values, step, val_quantity);
        ASSERT_TRUE(abs(true_res - paral_res) < 10e-5);
    } else {
        IntegralParallel(nullptr, 0, 0);
    }
}

TEST(Rectangle_Integral_Mpi, Test_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        double from = -15, to = 25;
        double true_res = TrueIntegral_X2(from, to);
        double step, val_quantity = 10e5;
        double *values = FuncValues_X2(from, to, val_quantity, &step);
        double paral_res = IntegralParallel(values, step, val_quantity);
        ASSERT_TRUE(abs(true_res - paral_res) < 10e-5);
    } else {
        IntegralParallel(nullptr, 0, 0);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
