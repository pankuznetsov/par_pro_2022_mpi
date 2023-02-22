// Copyright 2022 Asoskov Mikhail
#include "../../../modules/task_2/asoskov_m_scatter/scatter.h"
#include <cstring>

int scatter(void *sendbuf, int sendcount, MPI_Datatype sendtype,
            void *recvbuf, int recvcount, MPI_Datatype recvtype,
            int root, MPI_Comm comm) {
    if (root < 0 || sendcount <= 0 || recvcount <= 0)
        return MPI_ERR_COUNT;
    int sendtype_size, recvtype_size;
    MPI_Type_size(sendtype, &sendtype_size);
    MPI_Type_size(recvtype, &recvtype_size);
    int send_size = sendcount * sendtype_size;
    int recv_size = recvcount * recvtype_size;
    if (send_size != recv_size) return MPI_ERR_COUNT;
    int proc_rank, proc_count;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    if (proc_count == 1) return MPI_ERR_COUNT;
    if (proc_rank == root) {
        memcpy(recvbuf, reinterpret_cast<int8_t*>(sendbuf) +
               root * recv_size, send_size);
        for (int i = 0; i < proc_count; i++) {
            if (i == root) continue;
            MPI_Send(reinterpret_cast<int8_t*>(sendbuf) + i * send_size,
                     sendcount, sendtype, i, 0, comm);
        }
    } else  
        MPI_Recv(recvbuf, recvcount, recvtype, root, 0,
                 comm, MPI_STATUS_IGNORE);
    return MPI_SUCCESS;
}
