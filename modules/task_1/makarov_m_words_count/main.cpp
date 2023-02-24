// Copyright 2023 Makarov Mikhail

#include <gtest/gtest.h>
#include <vector>
#include "./words_count.h"
#include <gtest-mpi-listener.hpp>

void RandomStringTest(int length) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string randStr;
    int seqWords, parWords;

    if (rank == 0) {
        randStr = getRandStr(length);
        // std::cout << "randStr:\n{" << randStr << "}\n";

        seqWords = countWordsSequential(randStr, length);
        // std::cout << "seqWords = " << seqWords << "\n";
    }

    parWords = countWordsParallel(randStr, length);

    if (rank == 0) {
        // std::cout << "parWords = " << parWords << "\n";
        EXPECT_EQ(seqWords, parWords);
    }
}

TEST(WordsCount, Test_WordsCount_1) { RandomStringTest(10); }

TEST(WordsCount, Test_WordsCount_2) { RandomStringTest(20); }

TEST(WordsCount, Test_WordsCount_3) { RandomStringTest(50); }

TEST(WordsCount, Test_WordsCount_4) { RandomStringTest(100); }

TEST(WordsCount, Test_WordsCount_5) { RandomStringTest(50000); }

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
