#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include "./moore.h"


TEST(Moors_Algorithm_MPI, Test_5) {
    int rank;
    int n = 5;
    bool cycle_flag = false,
        cycle_flag2 = false;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> g(n * n);
    if (rank == 0)
        graph = getRandomGraph(n);
    std::vector<int> resSeq(n);
    std::vector<int> resPar(n);
    MooreParallel(g, 0, resPar, &cycle_flag );
    if (rank == 0) {
        resSeq = MooreSequential(g, 0, &cycle_flag2);
        for (int i = 0; i < n; ++i) {
            ASSERT_EQ(resPar[i], resSeq[i]);
        }
    }
}

TEST(Moors_Algorithm_MPI, Test_10) {
    int rank;
    int n = 10;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
        graph = getRandomGraph(n);
    std::vector<int> resSeq(n);
    std::vector<int> resPar(n);
    MooreParallel(g, 0, resPar, &cycle_flag );
    if (rank == 0) {
        resSeq = MooreSequential(g, 0, &cycle_flag2);
        for (int i = 0; i < n; ++i) {
            ASSERT_EQ(resPar[i], resSeq[i]);
        }
    }
}

TEST(Moors_Algorithm_MPI, Test_100) {
    int rank;
    int n = 100;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> g(n * n);
    if (rank == 0)
        graph = getRandomGraph(n);
    std::vector<int> resSeq(n);
    std::vector<int> resPar(n);
    MooreParallel(g, 0, resPar, &cycle_flag );
    if (rank == 0) {
        resSeq = MooreSequential(g, 0, &cycle_flag2);
        for (int i = 0; i < n; ++i) {
            ASSERT_EQ(resPar[i], resSeq[i]);
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