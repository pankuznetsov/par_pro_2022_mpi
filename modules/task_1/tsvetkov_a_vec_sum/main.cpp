// Copyright 2022 Tsvetkov
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include <gtest-mpi-listener.hpp>
#include "./vector_sum.h"

TEST(Vector_Sum_MPI, Test_Sum_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    if (rank == 0) {
        vec = {10, 20, 30, 40, 50, 60, 70, 80, 90, 4};
    }
    int sum = getSumParallel(vec);
    if (rank == 0) {
        int reference_sum = 454;
        ASSERT_EQ(reference_sum, sum);
    }
}

TEST(Vector_Sum_MPI, Test_Sum_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    if (rank == 0) {
        vec = {10, 20, 30, 40, 50, 60, 70, 80, 90, 3};
    }
    if (rank == 0) {
        int reference_sum = 453;
        int sum = getSumSequential(vec);
        ASSERT_EQ(reference_sum, sum);
    }
}

TEST(Vector_Sum_MPI, Test_Sum_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    int size_vector = 200;

    if (rank == 0) {
        vec = getRandomVector(size_vector);
    }
    int res_par = getSumParallel(vec);
    if (rank == 0) {
        int res_seq = getSumSequential(vec);
        ASSERT_EQ(res_par, res_seq);
    }
}

TEST(Vector_Sum_MPI, Test_Sum_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    int size_vector = 200;
    double tp1, tp2, ts1, ts2;

    if (rank == 0) {
        vec = getRandomVector(size_vector);
        tp1 = MPI_Wtime();
    }
    int res_par = getSumParallel(vec);
    if (rank == 0) {
        tp2 = MPI_Wtime();
        ts1 = MPI_Wtime();
        int res_seq = getSumSequential(vec);
        ts2 = MPI_Wtime();
        ASSERT_EQ(res_par, res_seq);
        std::cout << "Test 200 elements" << '\n';
        std::cout << "Parallel time: " << (tp2 - tp1) << '\n';
        std::cout << "Sequential time: " << (ts2 - ts1) << '\n';
    }
}

TEST(Vector_Sum_MPI, Test_Sum_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    int size_vector = 5000;
    double tp1, tp2, ts1, ts2;

    if (rank == 0) {
        vec = getRandomVector(size_vector);
        tp1 = MPI_Wtime();
    }
    int res_par = getSumParallel(vec);
    if (rank == 0) {
        tp2 = MPI_Wtime();
        ts1 = MPI_Wtime();
        int res_seq = getSumSequential(vec);
        ts2 = MPI_Wtime();
        ASSERT_EQ(res_par, res_seq);
        std::cout << "Test 5000 elements" << '\n';
        std::cout << "Parallel time: " << (tp2 - tp1) << '\n';
        std::cout << "Sequential time: " << (ts2 - ts1) << '\n';
    }
}

TEST(Vector_Sum_MPI, Test_Sum_6) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> vec;
    int size_vector = 15000;
    double tp1, tp2, ts1, ts2;

    if (rank == 0) {
        vec = getRandomVector(size_vector);
        tp1 = MPI_Wtime();
    }
    int res_par = getSumParallel(vec);
    if (rank == 0) {
        tp2 = MPI_Wtime();
        ts1 = MPI_Wtime();
        int res_seq = getSumSequential(vec);
        ts2 = MPI_Wtime();
        ASSERT_EQ(res_par, res_seq);
        std::cout << "Test 15000 elements" << '\n';
        std::cout << "Parallel time: " << (tp2 - tp1) << '\n';
        std::cout << "Sequential time: " << (ts2 - ts1) << '\n';
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
