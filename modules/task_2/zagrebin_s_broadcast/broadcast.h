// Copyright 2022 Zagrebin S.

#ifndef MODULES_TASK_2_ZAGREBIN_S_BROADCAST_BROADCAST_H_
#define MODULES_TASK_2_ZAGREBIN_S_BROADCAST_BROADCAST_H_
#include <mpi.h>

void MyBcast(void* data, int c, MPI_Datatype t, int r, MPI_Comm comm);

#endif  // MODULES_TASK_2_ZAGREBIN_S_BROADCAST_BROADCAST_H_
