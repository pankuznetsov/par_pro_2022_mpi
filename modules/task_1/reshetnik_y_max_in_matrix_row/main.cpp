// Copyright 2022 Reshetnik Yana
#include <gtest/gtest.h>
#include <vector>
#include "./max_in_matrix_row.h"
#include <gtest-mpi-listener.hpp>

TEST(Max_In_Matrix_Row, Test_Simple) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* matrix = nullptr;
    const int size = 4;
    if (rank == 0) {
        matrix = new int[size * size] {
                2, 4, 1, 3,
                4, 3, 2, 1,
                3, 1, 4, 2,
                1, 2, 3, 4
        };
    }

    int* max = parallel_max_by_row(matrix, size, size);

    if (rank == 0) {
        for (int i = 0; i < size; i++)
            ASSERT_EQ(4, max[i]);

        delete[] max;
        delete[] matrix;
    }
}

TEST(Max_In_Matrix_Row, Test_Random_Sq_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int size = 4;
    int* matrix = nullptr;
    if (rank == 0) {
        matrix = get_random_matrix(size, size);
    }

    int* max = parallel_max_by_row(matrix, size, size);

    if (rank == 0) {
        int* seq_max = max_by_row(matrix, size, size);

        for (int i = 0; i < size; i++)
            ASSERT_EQ(seq_max[i], max[i]);

        delete[] max;
        delete[] seq_max;
        delete[] matrix;
    }
}

TEST(Max_In_Matrix_Row, Test_Random_Long_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int size_x = 20;
    const int size_y = 3;
    int* matrix = nullptr;
    if (rank == 0) {
        matrix = get_random_matrix(size_x, size_y);
    }

    int* max = parallel_max_by_row(matrix, size_x, size_y);

    if (rank == 0) {
        int* seq_max = max_by_row(matrix, size_x, size_y);

        for (int i = 0; i < size_y; i++)
            ASSERT_EQ(seq_max[i], max[i]);

        delete[] max;
        delete[] seq_max;
        delete[] matrix;
    }
}

TEST(Max_In_Matrix_Row, Test_Random_Short_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int size_x = 3;
    const int size_y = 20;
    int* matrix = nullptr;
    if (rank == 0) {
        matrix = get_random_matrix(size_x, size_y);
    }

    int* max = parallel_max_by_row(matrix, size_x, size_y);

    if (rank == 0) {
        int* seq_max = max_by_row(matrix, size_x, size_y);

        for (int i = 0; i < size_y; i++)
            ASSERT_EQ(seq_max[i], max[i]);

        delete[] max;
        delete[] seq_max;
        delete[] matrix;
    }
}

TEST(Max_In_Matrix_Row, Test_Random_Big_Matrix) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int size_x = 20;
    const int size_y = 20;
    int* matrix = nullptr;
    if (rank == 0) {
        matrix = get_random_matrix(size_x, size_y);
    }

    int* max = parallel_max_by_row(matrix, size_x, size_y);

    if (rank == 0) {
        int* seq_max = max_by_row(matrix, size_x, size_y);

        for (int i = 0; i < size_y; i++)
            ASSERT_EQ(seq_max[i], max[i]);

        delete[] max;
        delete[] seq_max;
        delete[] matrix;
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
