// Copyright 2023 Pankov Anatoliy
#include "../../../modules/task_2/pankov_a_allreduce/allreduce.h"

#include <cmath>
#include <cstring>
#include <stdexcept>

int MyAllreduce(void* sendbuf, void* recvbuf, int count, MPI_Datatype datatype,
                MPI_Op op, MPI_Comm comm) {
    int rank, comm_sz;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Null buffers are not allowed
    if (sendbuf == nullptr) {
        throw std::runtime_error("sendbuf was null :(");
    }
    if (recvbuf == nullptr) {
        throw std::runtime_error("recvbuf was null :(");
    }

    // Sending an empty message is not allowed
    if (count == 0) {
        throw std::runtime_error("Sending data with count = 0 is pointless :)");
    }

    // MyAllreduce supports only int, float and double
    if (datatype != MPI_INT && datatype != MPI_FLOAT &&
        datatype != MPI_DOUBLE) {
        throw std::runtime_error("Unsupported datatype :(");
    }

    // MyAllreduce supports only sum operation
    if (op != MPI_SUM) {
        throw std::runtime_error("Unsupported operation :(");
    }

    // We copy our data from sendbuf to recvbuf on every process
    if (datatype == MPI_INT) {
        std::memcpy(recvbuf, sendbuf, count * sizeof(int));
    } else if (datatype == MPI_FLOAT) {
        std::memcpy(recvbuf, sendbuf, count * sizeof(float));
    } else if (datatype == MPI_DOUBLE) {
        std::memcpy(recvbuf, sendbuf, count * sizeof(double));
    }

    int exp = static_cast<int>(std::log2(comm_sz));
    for (int i = 0; i <= exp; i++) {
        int delt = static_cast<int>(std::pow(2, i));
        if (comm_sz > delt) {
            for (int proc = delt; proc < comm_sz;
                 proc += static_cast<int>(std::pow(2, i + 1))) {
                if (rank == proc)
                    MPI_Send(recvbuf, count, datatype, proc - delt, rank,
                             comm);

                if (rank == proc - delt) {
                    MPI_Status stat;
                    if (datatype == MPI_INT) {
                        int* getbuf = new int[count];
                        MPI_Recv(getbuf, count, datatype, proc, proc, comm,
                                 &stat);
                        for (int k = 0; k < count; k++)
                            static_cast<int*>(recvbuf)[k] += getbuf[k];
                    } else if (datatype == MPI_FLOAT) {
                        float* getbuf = new float[count];
                        MPI_Recv(getbuf, count, datatype, proc, proc, comm,
                                 &stat);
                        for (int k = 0; k < count; k++)
                            static_cast<float*>(recvbuf)[k] += getbuf[k];
                    } else if (datatype == MPI_DOUBLE) {
                        double* getbuf = new double[count];
                        MPI_Recv(getbuf, count, datatype, proc, proc, comm,
                                 &stat);
                        for (int k = 0; k < count; k++)
                            static_cast<double*>(recvbuf)[k] += getbuf[k];
                    }
                }
            }
        }
    }

    if (rank == 0) {
        // Sending the result to all other processes
        for (int otherRank = 1; otherRank < comm_sz; otherRank++) {
            MPI_Send(recvbuf, count, datatype, otherRank, otherRank + comm_sz,
                     comm);
        }
    } else {
        // Receiving the result
        MPI_Status stat;
        MPI_Recv(recvbuf, count, datatype, 0, rank + comm_sz, comm, &stat);
    }

    return MPI_SUCCESS;
}
