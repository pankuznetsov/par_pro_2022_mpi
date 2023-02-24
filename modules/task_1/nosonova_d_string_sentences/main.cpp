// Copyright 2022 Nosonova Darina
#include <gtest/gtest.h>
#include "../../../modules/task_1/nosonova_d_string_sentences/string_sentences.h"
#include <gtest-mpi-listener.hpp>


TEST(String_Sentences_Mpi, String_Sentences_Test_1_10e2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = 10e2, chance = 5, answer_paral = -1, answer_sequence = -1;
    double time_parallel, time_sequence;

    char *str = nullptr;

    if (rank == 0)
        str = GetRandomString(size, chance);

    answer_paral = GetSentencesQuantityParallel(str, size, &time_parallel);

    if (rank == 0) {
        answer_sequence =
            GetSentencesQuantitySequence(str, size, &time_sequence);

        ASSERT_EQ(answer_paral, answer_sequence);
    }
}

TEST(String_Sentences_Mpi, String_Sentences_Test_2_10e3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = 10e3, chance = 10, answer_paral = -1, answer_sequence = -1;
    double time_parallel = 0, time_sequence = 0;

    char *str = nullptr;

    if (rank == 0)
        str = GetRandomString(size, chance);

    answer_paral = GetSentencesQuantityParallel(str, size, &time_parallel);

    if (rank == 0) {
        answer_sequence =
            GetSentencesQuantitySequence(str, size, &time_sequence);

        ASSERT_EQ(answer_paral, answer_sequence);
    }
}

TEST(String_Sentences_Mpi, String_Sentences_Test_3_10e4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = 10e4, chance = 15, answer_paral = -1, answer_sequence = -1;
    double time_parallel = 0, time_sequence = 0;

    char *str = nullptr;

    if (rank == 0)
        str = GetRandomString(size, chance);

    answer_paral = GetSentencesQuantityParallel(str, size, &time_parallel);

    if (rank == 0) {
        answer_sequence =
            GetSentencesQuantitySequence(str, size, &time_sequence);

        ASSERT_EQ(answer_paral, answer_sequence);
    }
}

TEST(String_Sentences_Mpi, String_Sentences_Test_4_10e5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = 10e5, chance = 20, answer_paral = -1, answer_sequence = -1;
    double time_parallel = 0, time_sequence = 0;

    char *str = nullptr;

    if (rank == 0)
        str = GetRandomString(size, chance);

    answer_paral = GetSentencesQuantityParallel(str, size, &time_parallel);

    if (rank == 0) {
        answer_sequence =
            GetSentencesQuantitySequence(str, size, &time_sequence);

        ASSERT_EQ(answer_paral, answer_sequence);
    }
}

TEST(String_Sentences_Mpi, String_Sentences_Test_5_10e6) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int size = 10e6, chance = 25, answer_paral = -1, answer_sequence = -1;
    double time_parallel = 0, time_sequence = 0;

    char *str = nullptr;

    if (rank == 0)
        str = GetRandomString(size, chance);

    answer_paral = GetSentencesQuantityParallel(str, size, &time_parallel);

    if (rank == 0) {
        answer_sequence =
            GetSentencesQuantitySequence(str, size, &time_sequence);

        ASSERT_EQ(answer_paral, answer_sequence);
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
