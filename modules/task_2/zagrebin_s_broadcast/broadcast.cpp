// Copyright 2022 Zagrebin S.

#include <mpi.h>
#include "../../../modules/task_2/zagrebin_s_broadcast/broadcast.h"

void MyBcast(void* data, int c, MPI_Datatype t, int r, MPI_Comm comm) {
    int size, rank, m = 1;
    MPI_Status status;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank != 0) {
        MPI_Recv(data, c, t, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, &status);
        while (m <= rank) m = m << 1;
    }

    while (rank + m <= size - 1) {
        MPI_Send(data, c, t, rank + m, 0, comm);
        m = m << 1;
    }
}
