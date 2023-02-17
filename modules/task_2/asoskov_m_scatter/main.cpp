// Copyright 2022 Asoskov Mikhail
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_2/asoskov_m_scatter/scatter.h"
#include <gtest-mpi-listener.hpp>


template<typename T>
bool test(int root, MPI_Datatype type) {
    const int SIZE = 5;
    int proc_rank, proc_count;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    int size = SIZE * proc_count;
    std::vector<T> src(size), dest(size), loc_src(SIZE);
    if (proc_rank == root)
        for (int i = 0; i < size; src[i++] = i++) {}
    scatter(src.data(), SIZE, type, loc_src.data(), SIZE, type,
            root, MPI_COMM_WORLD);
    MPI_Gather(loc_src.data(), SIZE, type, dest.data(), SIZE, type,
               root, MPI_COMM_WORLD);
    return proc_rank != root || src == dest;
}

TEST(MPI_parallel, scatter_int_test) {
    EXPECT_TRUE(test<int>(0, MPI_INT));
}

TEST(MPI_parallel, scatter_float_test) {
    EXPECT_TRUE(test<float>(0, MPI_FLOAT));
}

TEST(MPI_parallel, scatter_double_test) {
    EXPECT_TRUE(test<double>(0, MPI_DOUBLE));
}

TEST(MPI_parallel, scatter_root_1_test) {
    EXPECT_TRUE(test<int>(1, MPI_INT));
}

TEST(MPI_parallel, scatter_int_to_char_test) {
    int proc_rank, proc_count;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    int size = 5 * proc_count;
    std::vector<int> src(size), dest(size);
    std::vector<char> loc_src(20);
    if (proc_rank == 0)
        for (int i = 0; i < size; src[i++] = i++) {}
    scatter(src.data(), 5, MPI_INT, loc_src.data(), 20, MPI_CHAR,
            0, MPI_COMM_WORLD);
    MPI_Gather(loc_src.data(), 20, MPI_CHAR, dest.data(), 5, MPI_INT,
               0, MPI_COMM_WORLD);
    EXPECT_TRUE(proc_rank || src == dest);
}

TEST(MPI_parallel, scatter_error_with_different_sizes) {
    int proc_rank, proc_count;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    std::vector<int> a(5), b(3);
    EXPECT_EQ(scatter(a.data(), 5, MPI_INT, b.data(), 3, MPI_INT,
                      0, MPI_COMM_WORLD), MPI_ERR_COUNT);
}

TEST(MPI_parallel, scatter_negative_root) {
    int proc_rank, proc_count;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    std::vector<int> a(5), b(5);
    EXPECT_EQ(scatter(a.data(), 5, MPI_INT, b.data(), 5, MPI_INT,
                      -1, MPI_COMM_WORLD), MPI_ERR_COUNT);
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
