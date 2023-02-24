// Copyright 2022 Zagrebin S.

#include <gtest/gtest.h>
#include <string>
#include "./sentence_count.h"
#include <gtest-mpi-listener.hpp>

TEST(Sentence_Count_MPI, One_string_test) {
    int rank, res; std::string data;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        data = "Just a sentence.";
    }

    countSentences(&res, data.c_str());

    if (rank == 0) {
        ASSERT_EQ(res, 1);
    }
}

TEST(Sentence_Count_MPI, No_dot_test) {
    int rank, res; std::string data;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        data = "Sentence must contain a dot";
    }

    countSentences(&res, data.c_str());

    if (rank == 0) {
        ASSERT_EQ(res, 0);
    }
}

TEST(Sentence_Count_MPI, Dots_test) {
    int rank, res; std::string data;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        data = "...";
    }

    countSentences(&res, data.c_str());

    if (rank == 0) {
        ASSERT_EQ(res, 1);
    }
}

TEST(Sentence_Count_MPI, Short_sentence_test) {
    int rank, res; std::string data;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        data = ".";
    }

    countSentences(&res, data.c_str());

    if (rank == 0) {
        ASSERT_EQ(res, 1);
    }
}

TEST(Sentence_Count_MPI, Compicated_test) {
    int rank, res; std::string data;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        data = "So, this is a sentence. And this is too... But the third ends with terminal symbol.";
    }

    countSentences(&res, data.c_str());

    if (rank == 0) {
        ASSERT_EQ(res, 3);
    }
}

int main(int argc, char** argv) {
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
