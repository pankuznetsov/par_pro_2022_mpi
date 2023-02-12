// Copyright 2023 Bezrukov Aleksandr
#include <gtest/gtest.h>
#include <vector>
#include "./radix_sort.h"
#include <gtest-mpi-listener.hpp>

void Test_Radix_Sort(int size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<double> originalVector;
    std::vector<double> parallelSortedVector;
    std::vector<double> sequentialSortedVector;

    double t1_s, t2_s, t1_p, t2_p;

    if (rank == 0) {
        originalVector = genRandomVector(size);
        parallelSortedVector = originalVector;
        sequentialSortedVector = originalVector;

        t1_s = MPI_Wtime();
        radixSortSequential(&sequentialSortedVector);
        t2_s = MPI_Wtime();
        t1_p = t2_s;

        /* std::cout << "originalVector:\n";
        printVector(originalVector);
        std::cout << "\n"; */
    }

    radixSortParallel(&parallelSortedVector, size);

    if (rank == 0) {
        t2_p = MPI_Wtime();
        EXPECT_EQ(parallelSortedVector, sequentialSortedVector);
        /* std::cout << "sequentialSortedVector:\n";
        vecPrint(sequentialSortedVector);
        std::cout << "\n\n";
        std::cout << "parallelSortedVector:\n";
        vecPrint(parallelSortedVector);

        std::cout << "\nSaved time: " << (t2_s - t1_s) - (t2_p - t1_p)
                  << "s\n"; */
    }
}

TEST(Tests_Radix_Sort, Test_1) { Test_Radix_Sort(2); }

TEST(Tests_Radix_Sort, Test_2) { Test_Radix_Sort(30); }

TEST(Tests_Radix_Sort, Test_3) { Test_Radix_Sort(100); }

TEST(Tests_Radix_Sort, Test_4) { Test_Radix_Sort(2000); }

TEST(Tests_Radix_Sort, Test_5) { Test_Radix_Sort(20000); }

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
