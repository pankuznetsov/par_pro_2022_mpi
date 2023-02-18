// Copyright 2022 Lapin Dmitriy
#ifndef MODULES_TASK_2_LAPIN_D_GRAPH_RING_OPS_MPI_H_
#define MODULES_TASK_2_LAPIN_D_GRAPH_RING_OPS_MPI_H_
#include <vector>
#include <string>
#include <mpi.h>


void MPI_group_ring_create(MPI_Comm old_comm, int nodes, int index[], MPI_Comm* new_comm);
int Ring_Send(const void* buf, int count, MPI_Datatype datatype, int from, int dest, int tag, MPI_Comm comm);


#endif  
