// Copyright 2023 Kuleva Anna
#include <gtest/gtest.h>
#include <vector>
#include "./check_lex_ordering.h"
#include <gtest-mpi-listener.hpp>

TEST(Generate_Random_String_MPI, Test_Negative_length) {
    ASSERT_ANY_THROW(getRandomString(-1));
}

TEST(Parallel_Operations_MPI, Parallel_Operations_MPI_gives_correct_answer) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str1 = "abc";
    std::string str2 = "bca";
    size_t len = std::min(str1.length(), str2.length());
    int res1 = getParallelOperations(str1, str2, len);
    int res2 = -1;
    if (rank == 0) {
        ASSERT_EQ(res1, res2);
    }
}

TEST(Sequential_Operations_MPI, Sequential_Operations_MPI_gives_correct_answer) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str1 = "abc";
    std::string str2 = "bca";
    size_t len = std::min(str1.length(), str2.length());
    int res1 = getParallelOperations(str1, str2, len);
    int res2 = -1;
    if (rank == 0) {
        ASSERT_EQ(res1, res2);
    }
}

TEST(Parallel_Operations_MPI, Comparing_strings_with_different_sizes) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str1 = "aaa";
    std::string str2 = str1;
    size_t len = std::min(str1.length(), str2.length());
    str2.push_back('a');
    int res1 = getParallelOperations(str1, str2, len);
    int res2 = -1;
    if (rank == 0) {
        ASSERT_EQ(res1, res2);
    }
}

TEST(Parallel_Operations_MPI, Large_string_comparison_equal) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str1 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    std::string str2 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    size_t len = std::min(str1.length(), str2.length());
    int res1 = getParallelOperations(str1, str2, len);
    int res2 = 0;
    if (rank == 0) {
        ASSERT_EQ(res1, res2);
    }
}

TEST(Parallel_Operations_MPI, Large_string_comparison1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str1 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    std::string str2 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab";
    size_t len = std::min(str1.length(), str2.length());
    int res1 = getParallelOperations(str1, str2, len);
    int res2 = -1;
    if (rank == 0) {
        ASSERT_EQ(res1, res2);
    }
}

TEST(Parallel_Operations_MPI, Large_string_comparison2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str1 = "aaaaaaaaaaaaaaaaaaaaabaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    std::string str2 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    size_t len = std::min(str1.length(), str2.length());
    int res1 = getParallelOperations(str1, str2, len);
    int res2 = 1;
    if (rank == 0) {
        ASSERT_EQ(res1, res2);
    }
}

TEST(Parallel_Operations_MPI, Parallel_Operations_MPI_for_random_generated_strings) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::string str1, str2;
    const int len = 10;
    if (rank == 0) {
        str1 = getRandomString(len);
        str2 = getRandomString(len);
    }
    int res1 = getParallelOperations(str1, str2, len);
    if (rank == 0) {
        int res2 = getSequentialOperations(str1, str2);
        ASSERT_EQ(res1, res2);
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
