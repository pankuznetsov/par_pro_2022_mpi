// Copyright 2022 Shvandyreva Alina
#include <mpi.h>
#include <cstring>
#include "../../../modules/task_2/shvandyreva_a_scatter/scatter.h"

int USER_Scatter(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, int recvcount, MPI_Datatype recvtype,
                int root, MPI_Comm comm) {
    if (sendcount != recvcount) {
        return MPI_ERR_COUNT;
    }

    if (sendtype != recvtype) {
        return MPI_ERR_TYPE;
    }

    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank);

    int sendSize, recvSize;
    MPI_Type_size(sendtype, &sendSize);
    MPI_Type_size(recvtype, &recvSize);

    if (root >= size) {
        return MPI_ERR_ROOT;
    }

    if (rank == root) {
        std::memcpy(recvbuf, reinterpret_cast<char*>(sendbuf) + root * sendcount * recvSize, sendcount * sendSize);
        for (int proc = 0; proc < size; ++proc) {
            if (proc == root) continue;
        MPI_Send(reinterpret_cast<char*>(sendbuf) + proc * sendcount * sendcount, sendcount, sendtype, proc, 0, comm);
        }
    } else {
        MPI_Status status;
        MPI_Recv(recvbuf, recvcount, recvtype, root, 0, comm, &status);
    }

    return MPI_SUCCESS;
}
