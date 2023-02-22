// Copyright 2022 Lapin Dmitriy
#include <gtest/gtest.h>
#include <vector>
#include "./ops_mpi.h"
#include <gtest-mpi-listener.hpp>

TEST(Parallel_Operations_MPI, Test_0) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size >= 3) {
        MPI_Comm ring_comm;
        int index[3] = { 0, 1, 2};
        MPI_group_ring_create(MPI_COMM_WORLD, 3,
            index, &ring_comm);

        int ring_rank, ring_size;
        if (ring_comm != MPI_COMM_NULL) {
            MPI_Comm_rank(ring_comm, &ring_rank);
            MPI_Comm_size(ring_comm, &ring_size);

            EXPECT_LT(ring_rank, ring_size);
            EXPECT_LT(ring_rank, 3);
        }
        if (ring_comm != MPI_COMM_NULL) {
            MPI_Comm_free(&ring_comm);
        }
    }
}


TEST(Parallel_Operations_MPI, Test_1) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size >= 3) {
        MPI_Comm ring_comm;
        int index[3] = { 0, 1, 2 };
        MPI_group_ring_create(MPI_COMM_WORLD, 3, index, &ring_comm);

        int ring_rank, ring_size;
        if (ring_comm != MPI_COMM_NULL) {
            MPI_Comm_rank(ring_comm, &ring_rank);
            MPI_Comm_size(ring_comm, &ring_size);

            double check[3];

            if (ring_rank == 0) {
                for (int i = 0; i < 3; i++) {
                    check[i] = (double)i;
                }
            }

            Ring_Send(check, 3, MPI_DOUBLE, 0, 2, 1, ring_comm);

            if (ring_rank == 2) {
                double check2[3];
                MPI_Status status;
                MPI_Recv(check2, 3, MPI_DOUBLE, MPI_ANY_SOURCE, 1, ring_comm, &status);
                for (int i = 0; i < 3; i++) {
                    EXPECT_EQ(check2[i], i);
                }
            }
        }

        if (ring_comm != MPI_COMM_NULL) {
            MPI_Comm_free(&ring_comm);
        }
    }
}


TEST(Parallel_Operations_MPI, Test_2) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size >= 3) {
        MPI_Comm ring_comm;
        int index[3] = { 0, 1, 2 };
        MPI_group_ring_create(MPI_COMM_WORLD, 3, index, &ring_comm);

        int ring_rank, ring_size;
        if (ring_comm != MPI_COMM_NULL) {
            MPI_Comm_rank(ring_comm, &ring_rank);
            MPI_Comm_size(ring_comm, &ring_size);

            int check[3];

            if (ring_rank == 0) {
                for (int i = 0; i < 3; i++) {
                    check[i] = i;
                }
            }

            Ring_Send(check, 3, MPI_INT, 0, 2, 1, ring_comm);

            if (ring_rank == 2) {
                int check2[3];
                MPI_Status status;
                MPI_Recv(check2, 3, MPI_INT, MPI_ANY_SOURCE, 1, ring_comm, &status);
                for (int i = 0; i < 3; i++) {
                    EXPECT_EQ(check2[i], i);
                }
            }
        }

        if (ring_comm != MPI_COMM_NULL) {
            MPI_Comm_free(&ring_comm);
        }
    }
}


TEST(Parallel_Operations_MPI, Test_3) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size >= 3) {
        MPI_Comm ring_comm;
        int index[3] = { 0, 1, 2 };
        MPI_group_ring_create(MPI_COMM_WORLD, 3, index, &ring_comm);

        int ring_rank, ring_size;
        if (ring_comm != MPI_COMM_NULL) {
            MPI_Comm_rank(ring_comm, &ring_rank);
            MPI_Comm_size(ring_comm, &ring_size);

            char check[3];

            if (ring_rank == 0) {
                for (int i = 0; i < 3; i++) {
                    check[i] = 'a';
                }
            }

            Ring_Send(check, 3, MPI_CHAR, 0, 2, 1, ring_comm);

            if (ring_rank == 2) {
                char check2[3];
                MPI_Status status;
                MPI_Recv(check2, 3, MPI_CHAR, MPI_ANY_SOURCE, 1, ring_comm, &status);
                for (int i = 0; i < 3; i++) {
                    EXPECT_EQ(check2[i], 'a');
                }
            }
        }

        if (ring_comm != MPI_COMM_NULL) {
            MPI_Comm_free(&ring_comm);
        }
    }
}




TEST(Parallel_Operations_MPI, Test_4) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size >= 4) {
        MPI_Comm ring_comm;
        int index[4] = { 0, 1, 2, 3 };
        MPI_group_ring_create(MPI_COMM_WORLD, 4, index, &ring_comm);

        int ring_rank, ring_size;
        if (ring_comm != MPI_COMM_NULL) {
            MPI_Comm_rank(ring_comm, &ring_rank);
            MPI_Comm_size(ring_comm, &ring_size);

            int check[3];

            if (ring_rank == 0) {
                for (int i = 0; i < 3; i++) {
                    check[i] = i;
                }
            }

            Ring_Send(check, 3, MPI_INT, 0, 3, 1, ring_comm);

            if (ring_rank == 3) {
                int check2[3];
                MPI_Status status;
                MPI_Recv(check2, 3, MPI_INT, MPI_ANY_SOURCE, 1, ring_comm, &status);
                for (int i = 0; i < 3; i++) {
                    EXPECT_EQ(check2[i], i);
                }
            }
        }

        if (ring_comm != MPI_COMM_NULL) {
            MPI_Comm_free(&ring_comm);
        }
    }
}

TEST(Parallel_Operations_MPI, Test_5) {
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size >= 6) {
        MPI_Comm ring_comm;
        int index[6] = { 0, 1, 2, 3, 4 ,5 };
        MPI_group_ring_create(MPI_COMM_WORLD, 6, index, &ring_comm);

        int ring_rank, ring_size;
        if (ring_comm != MPI_COMM_NULL) {
            MPI_Comm_rank(ring_comm, &ring_rank);
            MPI_Comm_size(ring_comm, &ring_size);

            int check[3];

            if (ring_rank == 0) {
                for (int i = 0; i < 3; i++) {
                    check[i] = i;
                }
            }

            Ring_Send(check, 3, MPI_INT, 0, 4, 1, ring_comm);

            if (ring_rank == 4) {
                int check2[3];
                MPI_Status status;
                MPI_Recv(check2, 3, MPI_INT, MPI_ANY_SOURCE, 1, ring_comm, &status);
                for (int i = 0; i < 3; i++) {
                    EXPECT_EQ(check2[i], i);
                }
            }
        }

        if (ring_comm != MPI_COMM_NULL) {
            MPI_Comm_free(&ring_comm);
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
