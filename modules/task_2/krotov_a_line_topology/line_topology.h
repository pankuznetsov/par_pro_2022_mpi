// Copyright 2023 Krotov Aleksey
#ifndef MODULES_TASK_2_KROTOV_A_LINE_TOPOLOGY_LINE_TOPOLOGY_H_
#define MODULES_TASK_2_KROTOV_A_LINE_TOPOLOGY_LINE_TOPOLOGY_H_

#include <mpi.h>
#include <vector>

void vecPrint(const std::vector<int>& targetVec);

void LineSend(void* buf, int count, MPI_Datatype datatype, int from, int dest,
              int tag, MPI_Comm comm);

#endif  // MODULES_TASK_2_KROTOV_A_LINE_TOPOLOGY_LINE_TOPOLOGY_H_
