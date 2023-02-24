// Copyright 2022 Dryamina Kseniya
#include <mpi.h>
#include "../../../modules/task_2/dryamina_k_farm/farm.h"

MPI_Comm createFarm(const MPI_Comm old) {
    int size;
    MPI_Comm_size(old, &size);
    const int ndims = 1;
    int dims[1] = { size };
    int periods[1] = { 0 };
    int reorder = 0;
    MPI_Comm farm;
    MPI_Cart_create(old, ndims, dims, periods, reorder, &farm);
    return farm;
}
