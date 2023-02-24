// Copyright 2023 Kochetov Maksim
#include "../../modules/task_2/kochetov_m_readers_writers/readers_writers.h"

#include <mpi.h>

#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

const int OWNER_PROC = 0;

const int ACTION_READ = 1;
const int ACTION_INC = 2;

// criticalOwner owns critical data
void criticalOwner(int* criticalData, int requestsLimit) {
    int totalProcs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /// std::cout << "Owner, rank = " << rank << '\n';

    for (int reqCounter = 0; reqCounter < requestsLimit; reqCounter++) {
        int action = -1;

        MPI_Status st;
        MPI_Recv(&action, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &st);

        if (action == ACTION_READ) {
            // std::cout << "> Owner got read request, criticalData = "
            //           << (*criticalData) << '\n';

            MPI_Ssend(criticalData, 1, MPI_INT, st.MPI_SOURCE, 0,
                      MPI_COMM_WORLD);
        } else if (action == ACTION_INC) {
            (*criticalData) += 1;

            // std::cout << "> Owner received INCREMENT, now criticalData = "
            //           << (*criticalData) << '\n';
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

// Every reader wants to read critical data readsCount times
void reader(int readsCount) {
    int totalProcs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /// std::cout << "Reader, rank = " << rank << '\n';

    for (int counter = 0; counter < readsCount; counter++) {
        // 1. Requests data
        MPI_Ssend(&ACTION_READ, 1, MPI_INT, OWNER_PROC, 0, MPI_COMM_WORLD);
        // std::cout << "Reader " << rank << " requested data \n";

        // 2. Waits for data
        int criticalDataValue = -1;
        MPI_Status st;
        MPI_Recv(&criticalDataValue, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &st);

        // std::cout << "Reader " << rank << " received "
        //          << criticalDataValue << '\n';
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

// And every writer wants to edit critical data writesCount times
void writer(int writesCount) {
    int totalProcs, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /// std::cout << "Writer, rank = " << rank << '\n';

    for (int counter = 0; counter < writesCount; counter++) {
        // Sends command to increment
        MPI_Send(&ACTION_INC, 1, MPI_INT, OWNER_PROC, 0, MPI_COMM_WORLD);
        // std::cout << "Writer " << rank << " sended increment\n";
    }
    MPI_Barrier(MPI_COMM_WORLD);
}
