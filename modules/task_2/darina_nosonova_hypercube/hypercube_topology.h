// Copyright 2022 Nosonova Darina
#ifndef MODULES_TASK_2_DARINA_NOSONOVA_HYPERCUBE_HYPERCUBE_TOPOLOGY_H_
#define MODULES_TASK_2_DARINA_NOSONOVA_HYPERCUBE_HYPERCUBE_TOPOLOGY_H_

#include <mpi.h>

MPI_Comm createHypercube(int tempDims, int sizeHypercube);
bool testHypercubeTopDATA(MPI_Comm Comm, int tempDims, int sizeHypercube);
bool isHypercubeTop(MPI_Comm isComm, int tempDims, int sizeHypercube);

#endif  // MODULES_TASK_2_DARINA_NOSONOVA_HYPERCUBE_HYPERCUBE_TOPOLOGY_H_
