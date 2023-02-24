// Copyright Yablonskii Dmitriy

#include "../../../modules/task_2/yablonskii_d_star/star.h"

// Create new ///STAR/// topology
MPI_Comm makingstarComm(const MPI_Comm oldComm) {
    // Create & get points
    int points;
    MPI_Comm_size(oldComm, &points);

    // Create & add id
    int *id = new int[points];
    id[0] = points - 1;
    for (int i = 1; i < points; i++) {
        id[i] = id[0] + i;
    }

    // Create & add rebs
    int e_S = 2 * (points - 1);
    int *rebs = new int[e_S];
    int i = 0;
    for (; i < points - 1; i++) {
        rebs[i] = i + 1;
    }
    for (; i < e_S; i++) {
        rebs[i] = 0;
    }

    // Create remake
    int remake = 0;

    // Create ///STAR///
    MPI_Comm starComm;
    MPI_Graph_create(oldComm, points, id, rebs, remake, &starComm);

    return starComm;
}

bool boolstartop(const MPI_Comm curComm) {
    // Is MPI_GRAPH???
    int status;
    MPI_Topo_test(curComm, &status);
    if (status != MPI_GRAPH) {
        return false;
    }

    // Check e_S info
    int points;
    int new_reb;
    MPI_Graphdims_get(curComm, &points, &new_reb);

    int e_S = 2 * (points - 1);
    if (new_reb != e_S) {
        return false;
    }

    // Check graph info
    int *id = new int[points];
    int *rebs = new int[e_S];
    MPI_Graph_get(curComm, points, e_S, id, rebs);
    for (int i = 0; i < points; i++) {
        if (id[i] != i + points - 1) {
            return false;
        }
    }
    int i = 0;
    for (; i < points - 1; i++) {
        if (rebs[i] != i + 1) {
            return false;
        }
    }
    for (; i < e_S; i++) {
        if (rebs[i] != 0) {
            return false;
        }
    }

    return true;
}
