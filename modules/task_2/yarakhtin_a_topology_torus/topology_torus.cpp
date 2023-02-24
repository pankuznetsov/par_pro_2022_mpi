// Copyright 2022 Yarakhtin Anton
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_2/yarakhtin_a_topology_torus/topology_torus.h"

Vector2 getCoordinates(int rank, const Vector2& size) {
    Vector2 pos;
    if (rank >= size.x * size.y) {
        pos.x = -1;
        pos.y = -1;
        return pos;
    }

    pos.x = rank % size.x;
    pos.y = rank / size.x;

    return pos;
}

int getRank(const Vector2& coord, const Vector2& size) {
    return coord.x + coord.y * size.x;
}

TorusElement torusCreate(const Vector2& size) {
    TorusElement torus_element;
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    torus_element.pos = getCoordinates(rank, size);
    if (torus_element.pos.x == -1) {
        return torus_element;
    }

    torus_element.size.x = size.x;
    torus_element.size.y = size.y;

    MPI_Comm_split(MPI_COMM_WORLD, torus_element.pos.x, rank, &torus_element.comm_x);
    MPI_Comm_split(MPI_COMM_WORLD, torus_element.pos.y, rank, &torus_element.comm_y);

    return torus_element;
}

static MPI_Comm selectComm(int src, int nxt, int rank, const TorusElement& tor) {
    if (rank != src || rank != nxt) {
        return MPI_COMM_NULL;
    }

    auto s_pos = getCoordinates(src, tor.size);
    auto d_pos = getCoordinates(nxt, tor.size);

    if (s_pos.x == d_pos.x) {
        return tor.comm_x;
    } else {
        return tor.comm_y;
    }

    return MPI_COMM_NULL;
}

static void torusSendImpl(void* buf, int count, MPI_Datatype datatype,
                            int rank, int src, int nxt, int tag, const TorusElement& tor) {
    if (rank == src) {
        MPI_Send(buf, count, datatype, nxt, tag, MPI_COMM_WORLD);
    } else if (rank == nxt) {
        MPI_Status status;
        MPI_Recv(buf, count, datatype, src, tag, MPI_COMM_WORLD, &status);
    }
}

static int calculateNext(int source, int destination, const Vector2& size) {
    auto s_pos = getCoordinates(source, size);
    auto d_pos = getCoordinates(destination, size);

    Vector2 n_pos = s_pos;

    if (s_pos.y > d_pos.y) {
        if (s_pos.y - d_pos.y < d_pos.y - s_pos.y + size.y) {
            n_pos.y--;
        } else {
            n_pos.y++;
            if (n_pos.y == size.y) {
                n_pos.y = 0;
            }
        }
    } else if (s_pos.y < d_pos.y) {
        if (d_pos.y - s_pos.y < s_pos.y - d_pos.y + size.y) {
            n_pos.y++;
        } else {
            n_pos.y--;
            if (n_pos.y == -1) {
                n_pos.y = size.y - 1;
            }
        }
    } else if (s_pos.x > d_pos.x) {
        if (s_pos.x - d_pos.x < d_pos.x - s_pos.x + size.x) {
            n_pos.x--;
        } else {
            n_pos.x++;
            if (n_pos.x == size.x) {
                n_pos.x = 0;
            }
        }
    } else if (s_pos.x < d_pos.x) {
        if (d_pos.x - s_pos.x < s_pos.x - d_pos.x + size.x) {
            n_pos.x++;
        } else {
            n_pos.x--;
            if (n_pos.x == -1) {
                n_pos.x = size.x - 1;
            }
        }
    }
    return getRank(n_pos, size);
}

void torusSend(void* buf, int count, MPI_Datatype datatype, int src, int dst, int tag, const TorusElement& tor) {
    if (src == dst) {
        return;
    }

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int next = calculateNext(src, dst, tor.size);
    while (next != dst) {
        torusSendImpl(buf, count, datatype, rank, src, next, tag, tor);
        src = next;
        next = calculateNext(src, dst, tor.size);
    }
    if (next != src) {
        torusSendImpl(buf, count, datatype, rank, src, next, tag, tor);
    }
}
