// Copyright 2022 Shipilov Artem
#include <gtest/gtest.h>
#include <iostream>
#include <gtest-mpi-listener.hpp>
#include "../../../modules/task_2/shipilov_a_producers_consumers/shipilov_a_prod_cons.h"

using std::cout;
using std::endl;

void test_producers_consumers(int prod_num,
                              int prod_count,
                              int cons_num,
                              int cons_count,
                              int buffer_capacity) {
    int rank;
    int size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (prod_num + cons_num + 1 > size)
        cout << "Not enough processes: total - " << size << ", but producers - "
         << prod_num << ", consumers - " << cons_num << ", manager - 1" << endl;
    else
        producers_consumers(prod_num, prod_count, cons_num,
                            cons_count, buffer_capacity);
}

TEST(Producers_Consumers_MPI, Test_1) {
    test_producers_consumers(2, 2, 0, 0, 5);
}

TEST(Producers_Consumers_MPI, Test_2) {
    test_producers_consumers(1, 1, 1, 1, 5);
}

TEST(Producers_Consumers_MPI, Test_3) {
    test_producers_consumers(2, 1, 1, 2, 5);
}

TEST(Producers_Consumers_MPI, Test_4) {
    test_producers_consumers(2, 4, 1, 8, 3);
}

TEST(Producers_Consumers_MPI, Test_5) {
    test_producers_consumers(1, 6, 2, 3, 3);
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
