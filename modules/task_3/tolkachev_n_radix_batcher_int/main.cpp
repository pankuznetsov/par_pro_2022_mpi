// Copyright 2022 Tolkachev Nikita
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include "../../../modules/task_3/tolkachev_n_radix_batcher_int/radix_batcher_int.h"
#include <gtest-mpi-listener.hpp>


TEST(RADIX_BATCHER, Test_1) {
    int rank;
    int num;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    if (rank == 0) {
        vector<int> arr = RandomVec(100);
        intSort(&arr);
        for (int i = 0; i < arr.size() - 1; i++) {
            EXPECT_TRUE(arr[i] <= arr[i + 1]);
        }
    }
}

TEST(RADIX_BATCHER, Test_2) {
    int rank, num;
    int size = 8;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    if (rank == 0) {
        vector<int> vec1 = RandomVec(size);
        vector<int> vec2 = RandomVec(size);
        intSort(&vec1);
        intSort(&vec2);
        vector<int> res(2 * size);
        std::copy(vec1.begin(), vec1.end(), res.begin());
        std::copy(vec2.begin(), vec2.end(), res.begin() + size);
        BatchMerge(&res, res.size(), 0, 1);
        for (int i = 0; i < res.size() - 1; i++) {
            EXPECT_TRUE(res[i] <= res[i + 1]);
        }
    }
}

TEST(RADIX_BATCHER, Test_3) {
    int rank, num;
    int size = 8;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    vector<int> temp;
    if (rank == 0) {
        temp = RandomVec(size);
    }
    temp = ParSort(temp, size);
    if (rank == 0) {
        for (int i = 0; i < temp.size() - 1; i++) {
            EXPECT_TRUE(temp[i] <= temp[i + 1]);
        }
    }
}
TEST(RADIX_BATCHER, Test_4) {
    int rank, num;
    int size = 32;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    vector<int> temp;
    if (rank == 0) {
        temp = RandomVec(size);
    }
    temp = ParSort(temp, size);
    if (rank == 0) {
        for (int i = 0; i < temp.size() - 1; i++) {
            EXPECT_TRUE(temp[i] <= temp[i + 1]);
        }
    }
}

TEST(RADIX_BATCHER, Test_5) {
    int rank, num;
    int size = 16;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    vector<int> temp;
    if (rank == 0) {
        temp = RandomVec(size);
    }
    temp = ParSort(temp, size);
    if (rank == 0) {
        for (int i = 0; i < temp.size() - 1; i++) {
            EXPECT_TRUE(temp[i] <= temp[i + 1]);
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
