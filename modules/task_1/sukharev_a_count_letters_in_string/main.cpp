// Copyright 2022 Sukharev Artem

#include <gtest/gtest.h>
#include <string>
#include <gtest-mpi-listener.hpp>
#include "./count_letters_in_string.h"

TEST(Count_Letters_In_String_MPI, Test_Some_Letters_Zzzz) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string globalString = "";

  if (rank == 0) {
    globalString = "zebrazovetzybra";
  }

  int resPar = countLettersParallel(globalString, 'z');

  if (rank == 0) {
    int resSeq = countLettersSequential(globalString, 'z');
    ASSERT_EQ(resPar, resSeq);
  }
}

TEST(Count_Letters_In_String_MPI, Test_All_Letters) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string globalString = "";

  if (rank == 0) {
    globalString = "ooooooooooooo";
  }

  int resPar = countLettersParallel(globalString, 'o');

  if (rank == 0) {
    int resSeq = countLettersSequential(globalString, 'o');
    ASSERT_EQ(resPar, resSeq);
  }
}

TEST(Count_Letters_In_String_MPI, Test_Zero_Letters) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::string globalString = "";

  if (rank == 0) {
    globalString = std::string(1000, 'e');
  }

  int resPar = countLettersParallel(globalString, 'w');

  if (rank == 0) {
    int resSeq = countLettersSequential(globalString, 'w');
    ASSERT_EQ(resPar, resSeq);
  }
}

TEST(Count_Letters_In_String_MPI, Test_Random_String) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string globalString = "";
  char letter = 0;
  if (rank == 0) {
    letter = getRandomLetter();
    globalString = getRandomString(1000);
  }

  int resPar = countLettersParallel(globalString, letter);

  if (rank == 0) {
    int resSeq = countLettersSequential(globalString, letter);
    ASSERT_EQ(resPar, resSeq);
  }
}

TEST(Count_Letters_In_String_MPI, Test_Random_String_2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string globalString = "";
  char letter = 0;
  if (rank == 0) {
    letter = getRandomLetter();
    globalString = getRandomString(10000);
  }

  int resPar = countLettersParallel(globalString, letter);

  if (rank == 0) {
    int resSeq = countLettersSequential(globalString, letter);
    ASSERT_EQ(resPar, resSeq);
  }
}

TEST(Count_Letters_In_String_MPI, Test_Empty_String) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string globalString = "";
  char letter = 0;
  if (rank == 0) {
    letter = getRandomLetter();
  }

  int resPar = countLettersParallel(globalString, letter);

  if (rank == 0) {
    int resSeq = countLettersSequential(globalString, letter);
    ASSERT_EQ(resPar, resSeq);
  }
}

TEST(Count_Letters_In_String_MPI, Test_Small_Random_String) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::string globalString = "";
  char letter = 0;
  if (rank == 0) {
    letter = getRandomLetter();
    globalString = getRandomString(4);
  }

  int resPar = countLettersParallel(globalString, letter);

  if (rank == 0) {
    int resSeq = countLettersSequential(globalString, letter);
    ASSERT_EQ(resPar, resSeq);
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
