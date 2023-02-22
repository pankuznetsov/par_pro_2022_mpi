// Copyright 2023 Popov Andrey
#include <mpi.h>
#include<iostream>
#include "../../../modules/task_2/Popov_A_from_one_to_all_broadcast/from_one_to_all_broadcast.h"


int From_one_to_all_Bcast(void* buffer, int cnt, MPI_Datatype type, int r, MPI_Comm comm) {
    int Process_Num, Process_Rank, sz;
    MPI_Comm_size(comm, &Process_Num);
    MPI_Comm_rank(comm, &Process_Rank);
    MPI_Type_size(type, &sz);
    if ((r < 0) || (r >= Process_Num)) {
        return MPI_ERR_ROOT;
    }
    if (Process_Rank == r) {
        for (int i = 0; i < Process_Num; i++) {
            if (i != r) {
                MPI_Send(buffer, cnt, type, i, 0, comm);
            }
        }
    }
    if (Process_Rank != r) {
        MPI_Status status;
        MPI_Recv(buffer, cnt, type, r, 0, comm, &status);
    }
    return 0;
}
