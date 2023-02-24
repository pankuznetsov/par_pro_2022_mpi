// Copyright 2022 Yarakhtin Anton
#include <gtest/gtest.h>
#include <vector>
#include "./count_of_words.h"
#include <gtest-mpi-listener.hpp>

TEST(Count_Of_Words, Test_Simple) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str;
    const int size = 13;
    if (rank == 0) {
        str = "Simple string";
    }

    int count = getWordsCountParallel(str, size);

    if (rank == 0) {
        ASSERT_EQ(2, count);
    }
}

TEST(Count_Of_Words, Test_Random_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str;
    const int size = 120;
    if (rank == 0) {
        str = getRandomString(size);
    }

    int count = getWordsCountParallel(str, size);

    if (rank == 0) {
        int reference_count = getWordsCountSequentially(str, size);
        ASSERT_EQ(reference_count, count);
    }
}

TEST(Count_Of_Words, Test_Empty_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str;
    const int size = 0;
    if (rank == 0) {
        str = "";
    }

    int count = getWordsCountParallel(str, size);

    if (rank == 0) {
        ASSERT_EQ(0, count);
    }
}

TEST(Count_Of_Words, Test_String_Without_Words) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str;
    const int size = 17;
    if (rank == 0) {
        str = "                 ";
    }

    int count = getWordsCountParallel(str, size);

    if (rank == 0) {
        ASSERT_EQ(0, count);
    }
}

TEST(Count_Of_Words, Test_One_Word) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str;
    const int size = 45;
    if (rank == 0) {
        str = "Pneumonoultramicroscopicsilicovolcanoconiosis";
    }

    int count = getWordsCountParallel(str, size);

    if (rank == 0) {
        ASSERT_EQ(1, count);
    }
}

TEST(Count_Of_Words, Test_Short_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str;
    const int size = 1;
    if (rank == 0) {
        str = "i";
    }

    int count = getWordsCountParallel(str, size);

    if (rank == 0) {
        ASSERT_EQ(1, count);
    }
}

TEST(Count_Of_Words, Test_Large_String) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string str;
    const int size = 10000;
    if (rank == 0) {
        str = getRandomString(size);
    }

    int count = getWordsCountParallel(str, size);

    if (rank == 0) {
        int reference_count = getWordsCountSequentially(str, size);
        ASSERT_EQ(reference_count, count);
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
