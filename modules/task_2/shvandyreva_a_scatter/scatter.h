// Copyright 2022 Shvandyreva Alina
#ifndef MODULES_TASK_2_SHVANDYREVA_A_SCATTER_SCATTER_H_
#define MODULES_TASK_2_SHVANDYREVA_A_SCATTER_SCATTER_H_

#include <mpi.h>

int USER_Scatter(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                 void *recvbuf, int recvcount, MPI_Datatype recvtype,
                 int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_SHVANDYREVA_A_SCATTER_SCATTER_H_
