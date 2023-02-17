// Copyright 2023 Melnik Denis
#include <gtest/gtest.h>
#include "../../../modules/task_2/melnik_d_lent_horiz_scheme/lent_horiz_scheme.h"
#include <gtest-mpi-listener.hpp>


void TestTask(int size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        Matrix matr = Matrix::GetRandom(size, size);
        vector<int> vect = GetRandomVector(size);

        vector<int> seq = Sequence(matr, vect);
        vector<int> par = Parallel(matr, vect);

        ASSERT_EQ(seq, par);
    } else {
        Matrix m(1, 1);
        vector<int> v(1);
        Parallel(m, v);
    }
}

TEST(Sleeping_barber, one_chair) { TestTask(10); }

TEST(Sleeping_barber, two_chairs) { TestTask(20); }

TEST(Sleeping_barber, three_chairs) { TestTask(50); }

TEST(Sleeping_barber, four_chairs) { TestTask(100); }

TEST(Sleeping_barber, five_chairs) { TestTask(200); }

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
