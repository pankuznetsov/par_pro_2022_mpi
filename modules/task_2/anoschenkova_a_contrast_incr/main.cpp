// Copyright 2023 Anoschenkova Anna
#include <gtest/gtest.h>
#include "./contrast_incr.h"
#include <gtest-mpi-listener.hpp>

TEST(ContrastIncr, ContrastTest_1) {
    std::vector<int> a;
    int contrast = 25;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        a = genImage(5, 7);
        ASSERT_ANY_THROW(incrContrastParallel(a, 3, 2, contrast));
    }
}

TEST(ContrastIncr, ContrastTest_2) {
    int rows = 3, cols = 3, contrast = 20;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> a(rows * cols);
    std::vector<int> ans_seq(rows * cols);

    if (rank == 0) {
        a[0] = 150, a[1] = 128, a[2] = 100, a[3] = 200, a[4] = 175, a[5] = 100,
        a[6] = 50, a[7] = 250, a[8] = 200;
        ans_seq[0] = 150, ans_seq[1] = 123, ans_seq[2] = 90, ans_seq[3] = 210,
        ans_seq[4] = 180, ans_seq[5] = 90, ans_seq[6] = 29, ans_seq[7] = 255,
        ans_seq[8] = 210;
    }
    std::vector<int> ans(rows * cols);
    ans = incrContrastParallel(a, rows, cols, contrast);
    if (rank == 0) {
        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(ContrastIncr, ContrastTest_3) {
    int rows = 3, cols = 3, contrast = 60;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> a(rows * cols);
    std::vector<int> ans_seq(rows * cols);

    if (rank == 0) {
        a[0] = 150, a[1] = 128, a[2] = 100, a[3] = 200, a[4] = 175, a[5] = 100,
        a[6] = 50, a[7] = 250, a[8] = 200;
        ans_seq[0] = 150, ans_seq[1] = 114, ans_seq[2] = 70, ans_seq[3] = 230,
        ans_seq[4] = 190, ans_seq[5] = 70, ans_seq[6] = 0, ans_seq[7] = 255,
        ans_seq[8] = 230;
    }

    std::vector<int> ans(rows * cols);
    ans = incrContrastParallel(a, rows, cols, contrast);

    if (rank == 0) ASSERT_EQ(ans_seq, ans);
}

TEST(ContrastIncr, ContrastTest_4) {
    int rows = 170, cols = 180, contrast = 30;
    std::vector<int> a(rows * cols);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> ans(rows * cols);
    std::vector<int> ans_seq(rows * cols);

    if (rank == 0) a = genImage(rows, cols);

    ans = incrContrastParallel(a, rows, cols, contrast);

    if (rank == 0) {
        ans_seq = incrContrast(a, rows, cols, contrast);
        ASSERT_EQ(ans_seq, ans);
    }
}

TEST(ContrastIncr, ContrastTest_5) {
    int rows = 500, cols = 700, contrast = 80;
    std::vector<int> a(rows * cols);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> ans(rows * cols);
    std::vector<int> ans_seq(rows * cols);

    if (rank == 0) a = genImage(rows, cols);

    ans = incrContrastParallel(a, rows, cols, contrast);

    if (rank == 0) {
        ans_seq = incrContrast(a, rows, cols, contrast);
        ASSERT_EQ(ans_seq, ans);
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
