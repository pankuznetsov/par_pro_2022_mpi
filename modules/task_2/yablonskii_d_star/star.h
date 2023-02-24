// Copyright Yablonskii Dmitriy

#ifndef MODULES_TASK_2_YABLONSKII_D_STAR_STAR_H_
#define MODULES_TASK_2_YABLONSKII_D_STAR_STAR_H_
#include <mpi.h>
#include <iostream>
MPI_Comm makingstarComm(const MPI_Comm oldComm);
bool boolstartop(const MPI_Comm curComm);

#endif  // MODULES_TASK_2_YABLONSKII_D_STAR_STAR_H_
