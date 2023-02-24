// Copyright 2022 Grachev Valentin
#include <gtest/gtest.h>
#include "../../../modules/task_1/grachev_v_matrix_max/matrix_max.h"
#include <gtest-mpi-listener.hpp>


TEST(Matrix_Max_MPI, Test_Matrix_Max_1_10x10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    TMatrix *matrix = new TMatrix(1, 1);

    double timeParal, timeLinear;
    double resParal, resLinear;

    if (rank == 0) {
        matrix = new TMatrix(10, 10);
        matrix->FillRandom(-100, 100);
    }

    GetMatrixMaxParallel(*matrix, &timeParal, &resParal);
    if (rank == 0) {
        resLinear = GetMatrixMaxLinear(*matrix, &timeLinear);
        ASSERT_EQ(resLinear, resParal);
    }
}

TEST(Matrix_Max_MPI, Test_Matrix_Max_2_100x10) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    TMatrix *matrix = new TMatrix(1, 1);

    double timeParal, timeLinear;
    double resParal, resLinear;

    if (rank == 0) {
        matrix = new TMatrix(100, 10);
        matrix->FillRandom(-1000, 1000);
    }

    GetMatrixMaxParallel(*matrix, &timeParal, &resParal);

    if (rank == 0) {
        resLinear = GetMatrixMaxLinear(*matrix, &timeLinear);
        ASSERT_EQ(resLinear, resParal);
    }
}

TEST(Matrix_Max_MPI, Test_Matrix_Max_3_10x1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    TMatrix *matrix = new TMatrix(1, 1);

    double timeParal, timeLinear;
    double resParal, resLinear;

    if (rank == 0) {
        matrix = new TMatrix(10, 1000);
        matrix->FillRandom(-10000, 10000);
    }

    GetMatrixMaxParallel(*matrix, &timeParal, &resParal);

    if (rank == 0) {
        resLinear = GetMatrixMaxLinear(*matrix, &timeLinear);
        ASSERT_EQ(resLinear, resParal);
    }
}

TEST(Matrix_Max_MPI, Test_Matrix_Max_4_2500x2500) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    TMatrix *matrix = new TMatrix(1, 1);

    double timeParal, timeLinear;
    double resParal, resLinear;

    if (rank == 0) {
        matrix = new TMatrix(2500, 2500);
        matrix->FillRandom(-10000, 10000);
    }

    GetMatrixMaxParallel(*matrix, &timeParal, &resParal);

    if (rank == 0) {
        resLinear = GetMatrixMaxLinear(*matrix, &timeLinear);
        ASSERT_EQ(resLinear, resParal);
    }
}

TEST(Matrix_Max_MPI, Test_Matrix_Max_5_5000x5000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    TMatrix *matrix = new TMatrix(1, 1);

    double timeParal, timeLinear;
    double resParal, resLinear;

    if (rank == 0) {
        matrix = new TMatrix(5000, 5000);
        matrix->FillRandom(-10000, 10000);
    }

    GetMatrixMaxParallel(*matrix, &timeParal, &resParal);

    if (rank == 0) {
        resLinear = GetMatrixMaxLinear(*matrix, &timeLinear);
        ASSERT_EQ(resLinear, resParal);
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
