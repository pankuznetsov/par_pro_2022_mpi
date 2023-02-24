// Copyright 2022 Yarakhtin Anton
#include <gtest/gtest.h>
#include <vector>
#include "./cannons_algorithm.h"
#include <gtest-mpi-listener.hpp>

constexpr double epsilon = 0.0001;

TEST(Cannons_Multiplication, Simple_Test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Matrix A(0, 0);
    Matrix B(0, 0);
    if (rank == 0) {
        A = generateMatrix(6, 1);
        B = generateMatrix(5, 6);
    }
    auto C = multiplyParallel(A, B);
    if (rank == 0) {
        auto C1 = multiplySequentially(A, B);
        ASSERT_EQ(C.x_size, C1.x_size);
        ASSERT_EQ(C.y_size, C1.y_size);
        for (int i = 0; i < C.x_size * C.y_size; i++) {
            ASSERT_TRUE(C1.data[i] - epsilon < C.data[i] && C.data[i] < C1.data[i] + epsilon);
        }
    }
}

TEST(Cannons_Multiplication, Vector_By_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Matrix A(0, 0);
    Matrix B(0, 0);
    if (rank == 0) {
        A = generateMatrix(10, 1);
        B = generateMatrix(1, 10);
    }
    auto C = multiplyParallel(A, B);
    if (rank == 0) {
        auto C1 = multiplySequentially(A, B);
        for (int i = 0; i < C.x_size * C.y_size; i++) {
            ASSERT_TRUE(C1.data[i] - epsilon < C.data[i] && C.data[i] < C1.data[i] + epsilon);
        }
    }
}

TEST(Cannons_Multiplication, Scalar_By_Scalar) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Matrix A(0, 0);
    Matrix B(0, 0);
    if (rank == 0) {
        A = generateMatrix(1, 1);
        B = generateMatrix(1, 1);
    }
    auto C = multiplyParallel(A, B);
    if (rank == 0) {
        auto C1 = multiplySequentially(A, B);
        for (int i = 0; i < C.x_size * C.y_size; i++) {
            ASSERT_TRUE(C1.data[i] - epsilon < C.data[i] && C.data[i] < C1.data[i] + epsilon);
        }
    }
}

TEST(Cannons_Multiplication, Scalar_By_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Matrix A(0, 0);
    Matrix B(0, 0);
    if (rank == 0) {
        A = generateMatrix(1, 1);
        B = generateMatrix(10, 1);
    }
    auto C = multiplyParallel(A, B);
    if (rank == 0) {
        auto C1 = multiplySequentially(A, B);
        for (int i = 0; i < C.x_size * C.y_size; i++) {
            ASSERT_TRUE(C1.data[i] - epsilon < C.data[i] && C.data[i] < C1.data[i] + epsilon);
        }
    }
}

TEST(Cannons_Multiplication, Big_Matrix_By_Vector) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Matrix A(0, 0);
    Matrix B(0, 0);
    if (rank == 0) {
        A = generateMatrix(64, 1);
        B = generateMatrix(64, 64);
    }
    auto C = multiplyParallel(A, B);
    if (rank == 0) {
        auto C1 = multiplySequentially(A, B);
        for (int i = 0; i < C.x_size * C.y_size; i++) {
            ASSERT_TRUE(C1.data[i] - epsilon < C.data[i] && C.data[i] < C1.data[i] + epsilon);
        }
    }
}

TEST(Cannons_Multiplication, Big_Matrix_By_Big_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    Matrix A(0, 0);
    Matrix B(0, 0);
    if (rank == 0) {
        A = generateMatrix(64, 64);
        B = generateMatrix(64, 64);
    }
    auto C = multiplyParallel(A, B);
    if (rank == 0) {
        auto C1 = multiplySequentially(A, B);
        for (int i = 0; i < C.x_size * C.y_size; i++) {
            ASSERT_TRUE(C1.data[i] - epsilon < C.data[i] && C.data[i] < C1.data[i] + epsilon);
        }
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
