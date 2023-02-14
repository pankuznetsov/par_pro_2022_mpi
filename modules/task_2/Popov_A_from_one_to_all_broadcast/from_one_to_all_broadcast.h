// Copyright 2023 Popov Andrey
#ifndef MODULES_TASK_2_POPOV_A_FROM_ONE_TO_ALL_BROADCAST_FROM_ONE_TO_ALL_BROADCAST_H_
#define MODULES_TASK_2_POPOV_A_FROM_ONE_TO_ALL_BROADCAST_FROM_ONE_TO_ALL_BROADCAST_H_

#include <vector>
#include <string>

int From_one_to_all_Bcast(void* buffer, int cnt, MPI_Datatype type, int r, MPI_Comm comm);

#endif  // MODULES_TASK_2_POPOV_A_FROM_ONE_TO_ALL_BROADCAST_FROM_ONE_TO_ALL_BROADCAST_H_
