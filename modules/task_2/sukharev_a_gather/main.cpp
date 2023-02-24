// Copyright 2022 Sukharev Artem

#define DONT_PRINT_TIME

#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include <gtest-mpi-listener.hpp>
#include "./gather.h"

TEST(Gather_MPI, Test_Correct_IntVector) {
  int rank, procNum;
  std::random_device dev;
  std::mt19937 gen(dev());
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  std::vector<int> globalVector;
  int root = 0;
  int globalSize = 0;
  if (rank == 0) {
    root = gen() % procNum;
    globalSize = gen() % 10 + 1;
    globalSize = globalSize * procNum;
    globalVector = getRandomVectorInt(globalSize);
  }

  MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&globalSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int localSize = globalSize / procNum;

  std::vector<int> localVector(localSize);
  MPI_Scatter(globalVector.data(), localSize, MPI_INT, localVector.data(),
              localSize, MPI_INT, 0, MPI_COMM_WORLD);

  double timeImb, timeOwn;
  std::vector<int> recImbVector;
  std::vector<int> recOwnVector;
  if (rank == root) {
    recImbVector.resize(globalSize);
    recOwnVector.resize(globalSize);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeImb = MPI_Wtime();
  }

  MPI_Gather(localVector.data(), localSize, MPI_INT, recImbVector.data(),
             localSize, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeImb = MPI_Wtime() - timeImb;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeOwn = MPI_Wtime();
  }

  MY_Gather(localVector.data(), localSize, MPI_INT, recOwnVector.data(),
            localSize, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeOwn = MPI_Wtime() - timeOwn;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    ASSERT_EQ(recImbVector, recOwnVector);
    if (procNum > 16) {
      EXPECT_TRUE(timeImb * 8 > timeOwn);
    }
#ifndef DONT_PRINT_TIME
    std::cout << "Test_Correct_IntVector:\n";
    std::cout << "timeImb = " << timeImb << "\n";
    std::cout << "timeOwn = " << timeOwn << "\n";
#endif  // DONT_PRINT_TIME
  }
}
TEST(Gather_MPI, Test_Correct_FloatVector) {
  int rank, procNum;
  std::random_device dev;
  std::mt19937 gen(dev());
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  std::vector<float> globalVector;
  int root = 0;
  int globalSize = 0;
  if (rank == 0) {
    root = gen() % procNum;
    globalSize = gen() % 10 + 1;
    globalSize = globalSize * procNum;
    globalVector = getRandomVectorFloat(globalSize);
  }

  MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&globalSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int localSize = globalSize / procNum;

  std::vector<float> localVector(localSize);
  MPI_Scatter(globalVector.data(), localSize, MPI_FLOAT, localVector.data(),
              localSize, MPI_FLOAT, 0, MPI_COMM_WORLD);

  double timeImb, timeOwn;
  std::vector<float> recImbVector;
  std::vector<float> recOwnVector;
  if (rank == root) {
    recImbVector.resize(globalSize);
    recOwnVector.resize(globalSize);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeImb = MPI_Wtime();
  }

  MPI_Gather(localVector.data(), localSize, MPI_FLOAT, recImbVector.data(),
             localSize, MPI_FLOAT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeImb = MPI_Wtime() - timeImb;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeOwn = MPI_Wtime();
  }

  MY_Gather(localVector.data(), localSize, MPI_FLOAT, recOwnVector.data(),
            localSize, MPI_FLOAT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeOwn = MPI_Wtime() - timeOwn;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    ASSERT_EQ(recImbVector, recOwnVector);
    if (procNum > 16) {
      EXPECT_TRUE(timeImb * 8 > timeOwn);
    }
#ifndef DONT_PRINT_TIME
    std::cout << "Test_Correct_FloatVector:\n";
    std::cout << "timeImb = " << timeImb << "\n";
    std::cout << "timeOwn = " << timeOwn << "\n";
#endif  // DONT_PRINT_TIME
  }
}
TEST(Gather_MPI, Test_Correct_DoubleVector) {
  int rank, procNum;
  std::random_device dev;
  std::mt19937 gen(dev());
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  std::vector<double> globalVector;
  int root = 0;
  int globalSize = 0;
  if (rank == 0) {
    root = gen() % procNum;
    globalSize = gen() % 10 + 1;
    globalSize = globalSize * procNum;
    globalVector = getRandomVectorDouble(globalSize);
  }

  MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&globalSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int localSize = globalSize / procNum;

  std::vector<double> localVector(localSize);
  MPI_Scatter(globalVector.data(), localSize, MPI_DOUBLE, localVector.data(),
              localSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  double timeImb, timeOwn;
  std::vector<double> recImbVector;
  std::vector<double> recOwnVector;
  if (rank == root) {
    recImbVector.resize(globalSize);
    recOwnVector.resize(globalSize);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeImb = MPI_Wtime();
  }

  MPI_Gather(localVector.data(), localSize, MPI_DOUBLE, recImbVector.data(),
             localSize, MPI_DOUBLE, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeImb = MPI_Wtime() - timeImb;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeOwn = MPI_Wtime();
  }

  MY_Gather(localVector.data(), localSize, MPI_DOUBLE, recOwnVector.data(),
            localSize, MPI_DOUBLE, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeOwn = MPI_Wtime() - timeOwn;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    ASSERT_EQ(recImbVector, recOwnVector);
    if (procNum > 16) {
      EXPECT_TRUE(timeImb * 8 > timeOwn);
    }
#ifndef DONT_PRINT_TIME
    std::cout << "Test_Correct_DoubleVector:\n";
    std::cout << "timeImb = " << timeImb << "\n";
    std::cout << "timeOwn = " << timeOwn << "\n";
#endif  // DONT_PRINT_TIME
  }
}
TEST(Gather_MPI, Test_Correct_Big_IntVector) {
  int rank, procNum;
  std::random_device dev;
  std::mt19937 gen(dev());
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  std::vector<int> globalVector;
  int root = 0;
  int globalSize = 0;
  if (rank == 0) {
    root = gen() % procNum;
    globalSize = gen() % 1000 + 1;
    globalSize = globalSize * procNum;
    globalVector = getRandomVectorInt(globalSize);
  }

  MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&globalSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int localSize = globalSize / procNum;

  std::vector<int> localVector(localSize);
  MPI_Scatter(globalVector.data(), localSize, MPI_INT, localVector.data(),
              localSize, MPI_INT, 0, MPI_COMM_WORLD);

  double timeImb, timeOwn;
  std::vector<int> recImbVector;
  std::vector<int> recOwnVector;
  if (rank == root) {
    recImbVector.resize(globalSize);
    recOwnVector.resize(globalSize);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeImb = MPI_Wtime();
  }

  MPI_Gather(localVector.data(), localSize, MPI_INT, recImbVector.data(),
             localSize, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeImb = MPI_Wtime() - timeImb;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeOwn = MPI_Wtime();
  }

  MY_Gather(localVector.data(), localSize, MPI_INT, recOwnVector.data(),
            localSize, MPI_INT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeOwn = MPI_Wtime() - timeOwn;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    ASSERT_EQ(recImbVector, recOwnVector);
    if (procNum > 16) {
      EXPECT_TRUE(timeImb * 8 > timeOwn);
    }
#ifndef DONT_PRINT_TIME
    std::cout << "Test_Correct_Big_IntVector:\n";
    std::cout << "timeImb = " << timeImb << "\n";
    std::cout << "timeOwn = " << timeOwn << "\n";
#endif  // DONT_PRINT_TIME
  }
}
TEST(Gather_MPI, Test_Correct_Big_FloatVector) {
  int rank, procNum;
  std::random_device dev;
  std::mt19937 gen(dev());
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  std::vector<float> globalVector;
  int root = 0;
  int globalSize = 0;
  if (rank == 0) {
    root = gen() % procNum;
    globalSize = gen() % 1000 + 1;
    globalSize = globalSize * procNum;
    globalVector = getRandomVectorFloat(globalSize);
  }

  MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&globalSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int localSize = globalSize / procNum;

  std::vector<float> localVector(localSize);
  MPI_Scatter(globalVector.data(), localSize, MPI_FLOAT, localVector.data(),
              localSize, MPI_FLOAT, 0, MPI_COMM_WORLD);

  double timeImb, timeOwn;
  std::vector<float> recImbVector;
  std::vector<float> recOwnVector;
  if (rank == root) {
    recImbVector.resize(globalSize);
    recOwnVector.resize(globalSize);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeImb = MPI_Wtime();
  }

  MPI_Gather(localVector.data(), localSize, MPI_FLOAT, recImbVector.data(),
             localSize, MPI_FLOAT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeImb = MPI_Wtime() - timeImb;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeOwn = MPI_Wtime();
  }

  MY_Gather(localVector.data(), localSize, MPI_FLOAT, recOwnVector.data(),
            localSize, MPI_FLOAT, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeOwn = MPI_Wtime() - timeOwn;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    ASSERT_EQ(recImbVector, recOwnVector);
    if (procNum > 16) {
      EXPECT_TRUE(timeImb * 8 > timeOwn);
    }
#ifndef DONT_PRINT_TIME
    std::cout << "Test_Correct_Big_FloatVector:\n";
    std::cout << "timeImb = " << timeImb << "\n";
    std::cout << "timeOwn = " << timeOwn << "\n";
#endif  // DONT_PRINT_TIME
  }
}
TEST(Gather_MPI, Test_Correct_Big_DoubleVector) {
  int rank, procNum;
  std::random_device dev;
  std::mt19937 gen(dev());
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  std::vector<double> globalVector;
  int root = 0;
  int globalSize = 0;
  if (rank == 0) {
    root = gen() % procNum;
    globalSize = gen() % 1000 + 1;
    globalSize = globalSize * procNum;
    globalVector = getRandomVectorDouble(globalSize);
  }

  MPI_Bcast(&root, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&globalSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

  int localSize = globalSize / procNum;

  std::vector<double> localVector(localSize);
  MPI_Scatter(globalVector.data(), localSize, MPI_DOUBLE, localVector.data(),
              localSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  double timeImb, timeOwn;
  std::vector<double> recImbVector;
  std::vector<double> recOwnVector;
  if (rank == root) {
    recImbVector.resize(globalSize);
    recOwnVector.resize(globalSize);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeImb = MPI_Wtime();
  }

  MPI_Gather(localVector.data(), localSize, MPI_DOUBLE, recImbVector.data(),
             localSize, MPI_DOUBLE, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeImb = MPI_Wtime() - timeImb;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    timeOwn = MPI_Wtime();
  }

  MY_Gather(localVector.data(), localSize, MPI_DOUBLE, recOwnVector.data(),
            localSize, MPI_DOUBLE, root, MPI_COMM_WORLD);

  if (rank == root) {
    timeOwn = MPI_Wtime() - timeOwn;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == root) {
    ASSERT_EQ(recImbVector, recOwnVector);
    if (procNum > 16) {
      EXPECT_TRUE(timeImb * 8 > timeOwn);
    }
#ifndef DONT_PRINT_TIME
    std::cout << "Test_Correct_Big_DoubleVector:\n";
    std::cout << "timeImb = " << timeImb << "\n";
    std::cout << "timeOwn = " << timeOwn << "\n";
#endif  // DONT_PRINT_TIME
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
