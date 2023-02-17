// Copyright 2022 Kudryavtsev Nikita
#include <gtest/gtest.h>
#include <mpi.h>
#include "../../../modules/task_2/kudryavtsev_n_sleeping_barber/sleeping_barber.h"
#include <gtest-mpi-listener.hpp>



TEST(Barber, TEST_ASSSERT1) {
    int visite_size;
    MPI_Comm_size(MPI_COMM_WORLD, &visite_size);
    if (visite_size < 2) {
        ASSERT_ANY_THROW(load());
    }
}

TEST(Barber, TEST_ASSSERT2) {
    int visite_size;
    MPI_Comm_size(MPI_COMM_WORLD, &visite_size);
    if (visite_size > 1) {
        ASSERT_NO_THROW(load());
    }
}

TEST(Barber, TEST_ASSSERT3) {
    int visite_size;
    MPI_Comm_size(MPI_COMM_WORLD, &visite_size);
    if (visite_size > 5) {
        ASSERT_NO_THROW(load());
    }
}

TEST(Barber, TEST_ASSSERT4) {
    int visite_size;
    MPI_Comm_size(MPI_COMM_WORLD, &visite_size);
    if (visite_size > 10) {
        ASSERT_NO_THROW(load());
    }
}

TEST(Barber, TEST_ASSSERT5) {
    int visite_size;
    MPI_Comm_size(MPI_COMM_WORLD, &visite_size);
    if (visite_size > 50) {
        ASSERT_NO_THROW(load());
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}

