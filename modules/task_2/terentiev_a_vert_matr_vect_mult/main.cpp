// Copyright 2023 Alexander Terentiev
#include <gtest/gtest.h>
#include "../../../modules/task_2/terentiev_a_vert_matr_vect_mult/vert_matr_vect_mult.h"
#include <gtest-mpi-listener.hpp>


void TestMultiple(int size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        Matrix matr(size, size);
        matr.Randomize();
        vector<int> vect = RandomVector(size);
        vector<int> seq = MultipleSequential(matr, vect);
        vector<int> par = MultipleParallel(matr, vect);
        ASSERT_EQ(par, seq);
    } else {
        Matrix m_dum(1, 1);
        vector<int> v_dum(1);
        MultipleParallel(m_dum, v_dum);
    }
}

TEST(Matr_Vect_Mult_Mpi, Test_5x5) { TestMultiple(5); }

TEST(Matr_Vect_Mult_Mpi, Test_10x10) { TestMultiple(10); }

TEST(Matr_Vect_Mult_Mpi, Test_50x50) { TestMultiple(50); }

TEST(Matr_Vect_Mult_Mpi, Test_100x100) { TestMultiple(100); }

TEST(Matr_Vect_Mult_Mpi, Test_200x200) { TestMultiple(200); }

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
