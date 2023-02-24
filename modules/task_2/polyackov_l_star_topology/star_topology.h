// Copyright 2022 Polyackov Lev
#ifndef MODULES_TASK_2_POLYACKOV_L_STAR_TOPOLOGY_STAR_TOPOLOGY_H_
#define MODULES_TASK_2_POLYACKOV_L_STAR_TOPOLOGY_STAR_TOPOLOGY_H_

#include <mpi.h>
#include <vector>
#include <string>
#include <iostream>

void MPI_Star_create(MPI_Comm* comm);
void MPI_Send_With_Star(void* message, int count, MPI_Datatype datatype, int from,
                        int dest, int tag, MPI_Comm comm, std::vector<int>* path);

#endif  // MODULES_TASK_2_POLYACKOV_L_STAR_TOPOLOGY_STAR_TOPOLOGY_H_
