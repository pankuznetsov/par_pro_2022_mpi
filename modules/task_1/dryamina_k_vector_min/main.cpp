// Copyright 2022 Dryamina Kseniya
#include <gtest/gtest.h>
#include <vector>
#include "./vector_min.h"
#include <gtest-mpi-listener.hpp>

TEST(Vector_Min_MPI, TEST_SIZE_100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int vecSize = 100;
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(vecSize);
    }

    int reductionParallel = vectorMinParallel(vec, vecSize);

    if (rank == 0) {
        int reductionSequential = vectorMinSequential(vec);
        ASSERT_EQ(reductionParallel, reductionSequential);
    }
}

TEST(Vector_Min_MPI, TEST_SIZE_512) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int vecSize = 512;
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(vecSize);
    }

    int reductionParallel = vectorMinParallel(vec, vecSize);

    if (rank == 0) {
        int reductionSequential = vectorMinSequential(vec);
        ASSERT_EQ(reductionParallel, reductionSequential);
    }
}

TEST(Vector_Min_MPI, TEST_SIZE_1024) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int vecSize = 1024;
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(vecSize);
    }

    int reductionParallel = vectorMinParallel(vec, vecSize);

    if (rank == 0) {
        int reductionSequential = vectorMinSequential(vec);
        ASSERT_EQ(reductionParallel, reductionSequential);
    }
}

TEST(Vector_Min_MPI, TEST_SIZE_2048) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int vecSize = 2048;
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(vecSize);
    }

    int reductionParallel = vectorMinParallel(vec, vecSize);

    if (rank == 0) {
        int reductionSequential = vectorMinSequential(vec);
        ASSERT_EQ(reductionParallel, reductionSequential);
    }
}

TEST(Vector_Min_MPI, TEST_SIZE_100000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int vecSize = 10000000;
    std::vector<int> vec;

    if (rank == 0) {
        vec = getRandomVector(vecSize);
    }

    int reductionParallel = vectorMinParallel(vec, vecSize);

    if (rank == 0) {
        int reductionSequential = vectorMinSequential(vec);
        ASSERT_EQ(reductionParallel, reductionSequential);
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
