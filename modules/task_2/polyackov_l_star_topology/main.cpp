// Copyright 2022 Polyackov Lev
#include <gtest/gtest.h>
#include <vector>
#include "./star_topology.h"
#include <gtest-mpi-listener.hpp>

TEST(Star_Topology_MPI, Can_Declare_Star_Topology) {
    MPI_Comm tmp;
    ASSERT_NO_THROW(MPI_Star_create(&tmp));
}

TEST(Star_Topology_MPI, Topology_Is_Graph) {
    int status;
    MPI_Comm comm;
    MPI_Star_create(&comm);
    MPI_Topo_test(comm, &status);
    ASSERT_EQ(status, MPI_GRAPH);
}

TEST(Star_Topology_MPI, Topology_Does_Not_Change_Num_Of_Processes) {
    int first_size;
    MPI_Comm_size(MPI_COMM_WORLD, &first_size);

    int second_size;
    MPI_Comm comm;
    MPI_Star_create(&comm);
    MPI_Comm_size(comm, &second_size);

    ASSERT_EQ(first_size, second_size);
}

TEST(Star_Topology_MPI, Topology_Is_Star_Topology) {
    MPI_Comm comm;
    MPI_Star_create(&comm);

    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    int numNeighbors;
    MPI_Graph_neighbors_count(comm, rank, &numNeighbors);

    if (rank == 0) {
        ASSERT_EQ(numNeighbors, size - 1);
        if (size == 1) {
            return;
        }
        std::vector<int> neighbors;
        neighbors.resize(numNeighbors);
        auto data_ptr = neighbors.data();
        MPI_Graph_neighbors(comm, rank, numNeighbors, data_ptr);
        for (int i = 0; i < numNeighbors; i++) {
            ASSERT_EQ(i + 1, *(data_ptr + i));
        }
    } else {
        ASSERT_EQ(numNeighbors, 1);
        std::vector<int> neighbors;
        neighbors.resize(numNeighbors);
        auto data_ptr = neighbors.data();
        MPI_Graph_neighbors(comm, rank, numNeighbors, data_ptr);
        for (int i = 0; i < numNeighbors; i++) {
            ASSERT_EQ(0, *(data_ptr + i));
        }
    }
}

TEST(Star_Topology_MPI, Can_Send_Message_With_Star_Topology) {
    MPI_Comm comm;
    MPI_Star_create(&comm);

    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    if (rank == 0 || size < 3) {
        return;
    }

    if (rank == 1) {
        std::vector<int> main_vec = { 5, 4, 3 };
        auto main_data_ptr = main_vec.data();
        int main_data_size = main_vec.size();
        int proc_num = size - 1;
        if (size == 2) {
            proc_num = 0;
        }
        MPI_Send(main_data_ptr, main_data_size, MPI_INT, proc_num, 0, comm);
    }

    if (rank == size - 1) {
        MPI_Status status;
        int sub_data_size = 3;
        std::vector<int> sub_vec;
        sub_vec.resize(sub_data_size);
        auto sub_data_ptr = sub_vec.data();
        MPI_Recv(sub_data_ptr, sub_data_size, MPI_INT, 1, 0, comm, &status);
        for (int i = 0; i < sub_data_size; i++) {
            ASSERT_EQ(sub_vec[i], 5 - i);
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
