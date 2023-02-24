// Copyright 2023 Kochetov Maksim
#include <gtest/gtest.h>
#include <vector>
#include "./readers_writers.h"
#include <gtest-mpi-listener.hpp>

TEST(ReadersWriters, Test_1) {
    // std::cout << ">>>>>>>>> Test 1 started\n";
    int readerReadCount = 2;
    int writerWriteCount = 2;

    int totalProcs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (totalProcs < 2) return;

    int readersWritersCount = totalProcs - 1;
    int readersCount = readersWritersCount / 2;
    int writersCount = readersWritersCount - readersCount;

    int criticalData;

    if (rank == 0) {
        // std::cout << ">>> writersCount = " << writersCount << '\n';
        // std::cout << ">>> readersCount = " << readersCount << '\n';

        criticalData = 0;

        criticalOwner(&criticalData, readersCount * readerReadCount +
                                         writersCount * writerWriteCount);
        ASSERT_EQ(criticalData, writersCount * writerWriteCount);
    } else if (rank <= readersWritersCount / 2) {
        reader(readerReadCount);
    } else {
        writer(writerWriteCount);
    }
}

TEST(ReadersWriters, Test_2) {
    // std::cout << ">>>>>>>>> Test 2 started\n";
    int readerReadCount = 2;
    int writerWriteCount = 2;

    int totalProcs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (totalProcs < 2) return;

    int readersWritersCount = totalProcs - 1;
    int readersCount = readersWritersCount / 2;
    int writersCount = readersWritersCount - readersCount;

    int criticalData;

    if (rank == 0) {
        // std::cout << ">>> writersCount = " << writersCount << '\n';
        // std::cout << ">>> readersCount = " << readersCount << '\n';

        criticalData = 0;

        criticalOwner(&criticalData, readersCount * readerReadCount +
                                         writersCount * writerWriteCount);
        ASSERT_EQ(criticalData, writersCount * writerWriteCount);
    } else if (rank <= readersWritersCount / 2) {
        reader(readerReadCount);
    } else {
        writer(writerWriteCount);
    }
}

TEST(ReadersWriters, Test_3) {
    // std::cout << ">>>>>>>>> Test 3 started\n";
    int readerReadCount = 3;
    int writerWriteCount = 3;

    int totalProcs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (totalProcs < 2) return;

    int readersWritersCount = totalProcs - 1;
    int readersCount = readersWritersCount / 2;
    int writersCount = readersWritersCount - readersCount;

    int criticalData;

    if (rank == 0) {
        // std::cout << ">>> writersCount = " << writersCount << '\n';
        // std::cout << ">>> readersCount = " << readersCount << '\n';

        criticalData = 0;

        criticalOwner(&criticalData, readersCount * readerReadCount +
                                         writersCount * writerWriteCount);

        ASSERT_EQ(criticalData, writersCount * writerWriteCount);
    } else if (rank <= readersWritersCount / 2) {
        reader(readerReadCount);
    } else {
        writer(writerWriteCount);
    }
}

TEST(ReadersWriters, Test_4) {
    // std::cout << ">>>>>>>>> Test 4 started\n";
    int readerReadCount = 4;
    int writerWriteCount = 4;

    int totalProcs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (totalProcs < 2) return;

    int readersWritersCount = totalProcs - 1;
    int readersCount = readersWritersCount / 2;
    int writersCount = readersWritersCount - readersCount;

    int criticalData;

    if (rank == 0) {
        // std::cout << ">>> writersCount = " << writersCount << '\n';
        // std::cout << ">>> readersCount = " << readersCount << '\n';

        criticalData = 0;

        criticalOwner(&criticalData, readersCount * readerReadCount +
                                         writersCount * writerWriteCount);

        ASSERT_EQ(criticalData, writersCount * writerWriteCount);
    } else if (rank <= readersWritersCount / 2) {
        reader(readerReadCount);
    } else {
        writer(writerWriteCount);
    }
}

TEST(ReadersWriters, Test_5) {
    // std::cout << ">>>>>>>>> Test 3 started\n";
    int readerReadCount = 5;
    int writerWriteCount = 5;

    int totalProcs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (totalProcs < 2) return;

    int readersWritersCount = totalProcs - 1;
    int readersCount = readersWritersCount / 2;
    int writersCount = readersWritersCount - readersCount;

    int criticalData;

    if (rank == 0) {
        // std::cout << ">>> writersCount = " << writersCount << '\n';
        // std::cout << ">>> readersCount = " << readersCount << '\n';

        criticalData = 0;

        criticalOwner(&criticalData, readersCount * readerReadCount +
                                         writersCount * writerWriteCount);

        ASSERT_EQ(criticalData, writersCount * writerWriteCount);
    } else if (rank <= readersWritersCount / 2) {
        reader(readerReadCount);
    } else {
        writer(writerWriteCount);
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
