// Copyright 2023 Popov Andrey
#include <gtest/gtest.h>
#include<mpi.h>
#include "./from_one_to_all_broadcast.h"
#include <gtest-mpi-listener.hpp>






TEST(Parallel_Count_Sentences_MPI, Bcast_INT) {
    int Process_Num, Process_Rank;
    int sz = 5;
    int r = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &Process_Rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Process_Num);
    std::vector<int> mas(sz);
    std::vector<int> mas1(sz);
    if (Process_Rank == r) {
        for (int i = 0; i < sz; i++) {
            mas[i] = i;
            mas1[i] = i;
        }
    }
    if (Process_Rank != r) {
        for (int i = 0; i < sz; i++) {
        }
    }
    MPI_Bcast(mas.data(), sz, MPI_INT, r, MPI_COMM_WORLD);
    From_one_to_all_Bcast(mas1.data(), sz, MPI_INT, r, MPI_COMM_WORLD);
    if (Process_Rank != r) {
        for (int i = 0; i < sz; i++) {
            ASSERT_EQ(mas[i], mas1[i]);
        }
    }
}

TEST(Parallel_Count_Sentences_MPI, Bcast_FLOAT) {
    int Process_Num, Process_Rank;
    int sz = 5;
    int r = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &Process_Rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Process_Num);
    std::vector<float> mas(sz);
    std::vector<float> mas1(sz);
    if (Process_Rank == r) {
        for (int i = 0; i < sz; i++) {
            mas[i] = i + 0.1 * i;
            mas1[i] = i + 0.1 * i;
        }
    }
    if (Process_Rank != r) {
        for (int i = 0; i < sz; i++) {
        }
    }
    MPI_Bcast(mas.data(), sz, MPI_FLOAT, r, MPI_COMM_WORLD);
    From_one_to_all_Bcast(mas1.data(), sz, MPI_FLOAT, r, MPI_COMM_WORLD);
    if (Process_Rank != r) {
        for (int i = 0; i < sz; i++) {
            ASSERT_EQ(mas[i], mas1[i]);
        }
    }
}

TEST(Parallel_Count_Sentences_MPI, Bcast_DOUBLE) {
    int Process_Num, Process_Rank;
    int sz = 5;
    int r = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &Process_Rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Process_Num);
    std::vector<double> mas(sz);
    std::vector<double> mas1(sz);
    if (Process_Rank == r) {
        for (int i = 0; i < sz; i++) {
            mas[i] = i + 0.1 * i;
            mas1[i] = i + 0.1 * i;
        }
    }
    if (Process_Rank != r) {
        for (int i = 0; i < sz; i++) {
        }
    }
    MPI_Bcast(mas.data(), sz, MPI_DOUBLE, r, MPI_COMM_WORLD);
    From_one_to_all_Bcast(mas1.data(), sz, MPI_DOUBLE, r, MPI_COMM_WORLD);
    if (Process_Rank != r) {
        for (int i = 0; i < sz; i++) {
            ASSERT_EQ(mas[i], mas1[i]);
        }
    }
}

TEST(Parallel_Count_Sentences_MPI, Bcast_FLOAT_BigSize) {
    int Process_Num, Process_Rank;
    int sz = 125;
    int r = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &Process_Rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Process_Num);
    std::vector<float> mas(sz);
    std::vector<float> mas1(sz);
    if (Process_Rank == r) {
        for (int i = 0; i < sz; i++) {
            mas[i] = i + 0.1 * i;
            mas1[i] = i + 0.1 * i;
        }
    }
    if (Process_Rank != r) {
        for (int i = 0; i < sz; i++) {
        }
    }
    MPI_Bcast(mas.data(), sz, MPI_FLOAT, r, MPI_COMM_WORLD);
    From_one_to_all_Bcast(mas1.data(), sz, MPI_FLOAT, r, MPI_COMM_WORLD);
    if (Process_Rank != r) {
        for (int i = 0; i < sz; i++) {
            ASSERT_EQ(mas[i], mas1[i]);
        }
    }
}

TEST(Parallel_Count_Sentences_MPI, Bcast_DOUBLE_BigSize) {
    int Process_Num, Process_Rank;
    int sz = 125;
    int r = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &Process_Rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Process_Num);
    std::vector<double> mas(sz);
    std::vector<double> mas1(sz);
    if (Process_Rank == r) {
        for (int i = 0; i < sz; i++) {
            mas[i] = i + 0.1 * i;
            mas1[i] = i + 0.1 * i;
        }
    }
    if (Process_Rank != r) {
        for (int i = 0; i < sz; i++) {
        }
    }
    MPI_Bcast(mas.data(), sz, MPI_DOUBLE, r, MPI_COMM_WORLD);
    From_one_to_all_Bcast(mas1.data(), sz, MPI_DOUBLE, r, MPI_COMM_WORLD);
    if (Process_Rank != r) {
        for (int i = 0; i < sz; i++) {
            ASSERT_EQ(mas[i], mas1[i]);
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
