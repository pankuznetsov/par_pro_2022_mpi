// Copyright Yablonskii Dmitriy

#include <gtest/gtest.h>
#include "../../../modules/task_2/yablonskii_d_star/star.h"
#include <gtest-mpi-listener.hpp>

TEST(Test_Star, make_star) {
    ASSERT_NO_THROW(makingstarComm(MPI_COMM_WORLD));
}

TEST(Test_Star, no_throw) {
    ASSERT_NO_THROW(boolstartop(MPI_COMM_WORLD));
}

TEST(Test_Star, rand_star_top) {
    int rank, points;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &points);

    int* id = new int[points];
    int* reb = new int[2 * points];
    for (int i = 0; i < points; i++) {
        id[i] = 2 + i * 2;
        reb[i * 2] = (points - 1 + i) % points;
        reb[(i * 2) + 1] = (points + 1 + i) % points;
    }
    MPI_Comm comm;
    MPI_Graph_create(MPI_COMM_WORLD, points, id, reb, 0, &comm);
    if (rank == 0 && points != 1) {
        EXPECT_FALSE(boolstartop(comm));
    }
}

TEST(Test_Star, t_is_star_top) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm star = makingstarComm(MPI_COMM_WORLD);
    if (rank == 0) {
        EXPECT_TRUE(boolstartop(star));
    }
}

TEST(Test_Star, t_not_star_top) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        EXPECT_FALSE(boolstartop(MPI_COMM_WORLD));
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
