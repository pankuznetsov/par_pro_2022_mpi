// Copyright 2022 Reshetnik Yana
#include <gtest/gtest.h>
#include <vector>
#include "./matr_mult_cannons.h"
#include <gtest-mpi-listener.hpp>

TEST(Cannons_Multiplication, Test_Simple) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int sz = 3;

    double* m1 = new double[sz * sz];
    double* m2 = new double[sz * sz];
    double* p = new double[sz * sz];

    if (rank == 0) {
        fill_matrix(m1, sz, sz);
        fill_matrix(m2, sz, sz);
    }
    cannon_parallel_multiply(p, m1, sz, sz, m2, sz, sz);
    if (rank == 0) {
        double* s = new double[sz * sz];
        multiply_matrix(s, m1, sz, sz, m2, sz, sz);
        for (int i = 0; i < sz * sz; i++)
            ASSERT_DOUBLE_EQ(s[i], p[i]);
        delete[] s;
    }

    delete[] m1;
    delete[] m2;
    delete[] p;
}

TEST(Cannons_Multiplication, Test_M1x1_M1x1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int m1c = 1;
    const int m1r = 1;
    const int m2c = 1;
    const int m2r = m1c;

    double* m1 = new double[m1c * m1r];
    double* m2 = new double[m2c * m2r];
    double* p = new double[m1r * m2c];

    if (rank == 0) {
        fill_matrix(m1, m1c, m1r);
        fill_matrix(m2, m2c, m2r);
    }
    cannon_parallel_multiply(p, m1, m1c, m1r, m2, m2c, m2r);
    if (rank == 0) {
        double* s = new double[m1r * m2c];
        multiply_matrix(s, m1, m1c, m1r, m2, m2c, m2r);
        for (int i = 0; i < m1r * m2c; i++)
            ASSERT_DOUBLE_EQ(s[i], p[i]);
        delete[] s;
    }

    delete[] m1;
    delete[] m2;
    delete[] p;
}

TEST(Cannons_Multiplication, Test_M7x1_M1x7) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int m1c = 7;
    const int m1r = 1;
    const int m2c = 1;
    const int m2r = m1c;

    double* m1 = new double[m1c * m1r];
    double* m2 = new double[m2c * m2r];
    double* p = new double[m1r * m2c];

    if (rank == 0) {
        fill_matrix(m1, m1c, m1r);
        fill_matrix(m2, m2c, m2r);
    }
    cannon_parallel_multiply(p, m1, m1c, m1r, m2, m2c, m2r);
    if (rank == 0) {
        double* s = new double[m1r * m2c];
        multiply_matrix(s, m1, m1c, m1r, m2, m2c, m2r);
        for (int i = 0; i < m1r * m2c; i++)
            ASSERT_DOUBLE_EQ(s[i], p[i]);
        delete[] s;
    }

    delete[] m1;
    delete[] m2;
    delete[] p;
}

TEST(Cannons_Multiplication, Test_M16x1_M16x16) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int m1c = 16;
    const int m1r = 1;
    const int m2c = 16;
    const int m2r = m1c;

    double* m1 = new double[m1c * m1r];
    double* m2 = new double[m2c * m2r];
    double* p = new double[m1r * m2c];

    if (rank == 0) {
        fill_matrix(m1, m1c, m1r);
        fill_matrix(m2, m2c, m2r);
    }
    cannon_parallel_multiply(p, m1, m1c, m1r, m2, m2c, m2r);
    if (rank == 0) {
        double* s = new double[m1r * m2c];
        multiply_matrix(s, m1, m1c, m1r, m2, m2c, m2r);
        for (int i = 0; i < m1r * m2c; i++)
            ASSERT_DOUBLE_EQ(s[i], p[i]);
        delete[] s;
    }

    delete[] m1;
    delete[] m2;
    delete[] p;
}

TEST(Cannons_Multiplication, Test_M16x16_M16x16) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int m1c = 16;
    const int m1r = 16;
    const int m2c = 16;
    const int m2r = m1c;

    double* m1 = new double[m1c * m1r];
    double* m2 = new double[m2c * m2r];
    double* p = new double[m1r * m2c];

    if (rank == 0) {
        fill_matrix(m1, m1c, m1r);
        fill_matrix(m2, m2c, m2r);
    }
    cannon_parallel_multiply(p, m1, m1c, m1r, m2, m2c, m2r);
    if (rank == 0) {
        double* s = new double[m1r * m2c];
        multiply_matrix(s, m1, m1c, m1r, m2, m2c, m2r);
        for (int i = 0; i < m1r * m2c; i++)
            ASSERT_DOUBLE_EQ(s[i], p[i]);
        delete[] s;
    }

    delete[] m1;
    delete[] m2;
    delete[] p;
}

TEST(Cannons_Multiplication, Test_M24x24_M24x24) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int m1c = 24;
    const int m1r = 24;
    const int m2c = 24;
    const int m2r = m1c;

    double* m1 = new double[m1c * m1r];
    double* m2 = new double[m2c * m2r];
    double* p = new double[m1r * m2c];

    if (rank == 0) {
        fill_matrix(m1, m1c, m1r);
        fill_matrix(m2, m2c, m2r);
    }
    cannon_parallel_multiply(p, m1, m1c, m1r, m2, m2c, m2r);
    if (rank == 0) {
        double* s = new double[m1r * m2c];
        multiply_matrix(s, m1, m1c, m1r, m2, m2c, m2r);
        for (int i = 0; i < m1r * m2c; i++)
            ASSERT_DOUBLE_EQ(s[i], p[i]);
        delete[] s;
    }

    delete[] m1;
    delete[] m2;
    delete[] p;
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
