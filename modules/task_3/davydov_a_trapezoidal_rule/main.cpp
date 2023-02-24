// Copyright 2022 Davydov Andrey
#include <gtest/gtest.h>
#include <cmath>
#include "./trapezoidal_rule.h"
#include <gtest-mpi-listener.hpp>

TEST(TRAPEZOIDAL_RULE, Plus) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double xa = 2.0;
    double ya = 6.0;

    double xb = 3.0;
    double yb = 9.0;

    int n = 64;

    double parallelResult = parallelTrapezoidalRule(xa, xb, ya, yb, n, plus);

    if (rank == 0) {
        double sequentialResult = sequentialTrapezoidalRule(xa, xb, ya, yb, n, plus);
        ASSERT_NEAR(parallelResult, sequentialResult, 0.001);
    }
}

TEST(TRAPEZOIDAL_RULE, Minus) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double xa = 2.0;
    double ya = 6.0;

    double xb = 3.0;
    double yb = 9.0;

    int n = 64;

    double parallelResult = parallelTrapezoidalRule(xa, xb, ya, yb, n, minus);

    if (rank == 0) {
        double sequentialResult = sequentialTrapezoidalRule(xa, xb, ya, yb, n, minus);
        ASSERT_NEAR(parallelResult, sequentialResult, 0.001);
    }
}

TEST(TRAPEZOIDAL_RULE, Multiple) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double xa = 2.0;
    double ya = 6.0;

    double xb = 3.0;
    double yb = 9.0;

    int n = 64;

    double parallelResult = parallelTrapezoidalRule(xa, xb, ya, yb, n, multiple);

    if (rank == 0) {
        double sequentialResult = sequentialTrapezoidalRule(xa, xb, ya, yb, n, multiple);
        ASSERT_NEAR(parallelResult, sequentialResult, 0.001);
    }
}

TEST(TRAPEZOIDAL_RULE, Division) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double xa = 2.0;
    double ya = 6.0;

    double xb = 3.0;
    double yb = 9.0;

    int n = 64;

    double parallelResult = parallelTrapezoidalRule(xa, xb, ya, yb, n, division);

    if (rank == 0) {
        double sequentialResult = sequentialTrapezoidalRule(xa, xb, ya, yb, n, division);
        ASSERT_NEAR(parallelResult, sequentialResult, 0.001);
    }
}

TEST(TRAPEZOIDAL_RULE, QuadSum) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double xa = 2.0;
    double ya = 6.0;

    double xb = 3.0;
    double yb = 9.0;

    int n = 64;

    double parallelResult = parallelTrapezoidalRule(xa, xb, ya, yb, n, quadSum);

    if (rank == 0) {
        double sequentialResult = sequentialTrapezoidalRule(xa, xb, ya, yb, n, quadSum);
        ASSERT_NEAR(parallelResult, sequentialResult, 0.001);
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
