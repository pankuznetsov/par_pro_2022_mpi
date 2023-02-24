// Copyright 2022 Yarakhtin Anton
#ifndef MODULES_TASK_2_YARAKHTIN_A_TOPOLOGY_TORUS_TOPOLOGY_TORUS_H_
#define MODULES_TASK_2_YARAKHTIN_A_TOPOLOGY_TORUS_TOPOLOGY_TORUS_H_

#include <mpi.h>
#include <vector>

struct Vector2 {
    int x;
    int y;
};

struct TorusElement {
    Vector2 pos = { 0, 0 };
    Vector2 size = { 0, 0 };
    MPI_Comm comm_x;
    MPI_Comm comm_y;
};

Vector2 getCoordinates(int rank, const Vector2& size);
int getRank(const Vector2& coord, const Vector2& size);
TorusElement torusCreate(const Vector2& size);
void torusSend(void* buf, int count, MPI_Datatype datatype, int src, int dst, int tag, const TorusElement& tor);

#endif  // MODULES_TASK_2_YARAKHTIN_A_TOPOLOGY_TORUS_TOPOLOGY_TORUS_H_
