// Copyright 2022 Tsvetkov
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include <gtest-mpi-listener.hpp>
#include "./producers_consumers.h"

TEST(Producers_Consumers_MPI, Test_1) {
  int ProcNum, ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  int res_size;
  int N1 = 0;  // consumers operations
  int N2 = 5;  // producers operations
  if (ProcRank == 0) {
    res_size = main_proc((N2 * (ProcNum / 2) + N1 * ((ProcNum - 1) / 2)));  // operations all
  }
  if (ProcRank != 0) {
    if (ProcRank % 2 == 0) {
      for (int i = 0; i < N1; i++) {
        consume();
      }
    }
    if (ProcRank % 2 == 1) {
      for (int i = 0; i < N2; i++) {
        produce();
      }
    }
  }
  if (ProcRank == 0) {
    ASSERT_EQ(res_size, (N2 * (ProcNum / 2) - N1 * ((ProcNum - 1) / 2)));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Producers_Consumers_MPI, Test_2) {
  int ProcNum, ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  int res_size;
  int N1 = 3;  // consumers operations
  int N2 = 7;  // producers operations
  if (ProcRank == 0) {
    res_size = main_proc((N2 * (ProcNum / 2) + N1 * ((ProcNum - 1) / 2)));  // operations all
  }
  if (ProcRank != 0) {
    if (ProcRank % 2 == 0) {
      for (int i = 0; i < N1; i++) {
        consume();
      }
    }
    if (ProcRank % 2 == 1) {
      for (int i = 0; i < N2; i++) {
        produce();
      }
    }
  }
  if (ProcRank == 0) {
    ASSERT_EQ(res_size, (N2 * (ProcNum / 2) - N1 * ((ProcNum - 1) / 2)));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Producers_Consumers_MPI, Test_3) {
  int ProcNum, ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  int res_size;
  int N1 = 2;  // consumers operations
  int N2 = 3;  // producers operations
  if (ProcRank == 0) {
    res_size = main_proc((N2 * (ProcNum / 2) + N1 * ((ProcNum - 1) / 2)));  // operations all
  }
  if (ProcRank != 0) {
    if (ProcRank % 2 == 0) {
      for (int i = 0; i < N1; i++) {
        consume();
      }
    }
    if (ProcRank % 2 == 1) {
      for (int i = 0; i < N2; i++) {
        produce();
      }
    }
  }
  if (ProcRank == 0) {
    ASSERT_EQ(res_size, (N2 * (ProcNum / 2) - N1 * ((ProcNum - 1) / 2)));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Producers_Consumers_MPI, Test_4) {
  int ProcNum, ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  int res_size;
  int N1 = 3;  // consumers operations
  int N2 = 5;  // producers operations
  if (ProcRank == 0) {
    res_size = main_proc((N2 * (ProcNum / 2) + N1 * ((ProcNum - 1) / 2)));
  }  // operations all
  if (ProcRank != 0) {
    if (ProcRank % 2 == 0) {
      for (int i = 0; i < N1; i++) {
        consume();
      }
    }
    if (ProcRank % 2 == 1) {
      for (int i = 0; i < N2; i++) {
        produce();
      }
    }
  }
  if (ProcRank == 0) {
    ASSERT_EQ(res_size, (N2 * (ProcNum / 2) - N1 * ((ProcNum - 1) / 2)));
  }
  MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Producers_Consumers_MPI, Test_5) {
  int ProcNum, ProcRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  int res_size;
  int N1 = 1;  // consumers operations
  int N2 = 2;  // producers operations
  if (ProcRank == 0) {
    res_size = main_proc((N2 * (ProcNum / 2) + N1 * ((ProcNum - 1) / 2)));  // operations all
  }
  if (ProcRank != 0) {
    if (ProcRank % 2 == 1) {
      for (int i = 0; i < N2; i++) {
        produce();
      }
    }
    if (ProcRank % 2 == 0) {
      for (int i = 0; i < N1; i++) {
        consume();
      }
    }
  }
  if (ProcRank == 0) {
    ASSERT_EQ(res_size, (N2 * (ProcNum / 2) - N1 * ((ProcNum - 1) / 2)));
  }
  MPI_Barrier(MPI_COMM_WORLD);
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
