// Copyright 2022 Lapin Dmitriy
#include <mpi.h>
#include <vector>
#include <iostream>
#include <list>
#include "./ops_mpi.h"
using namespace std;


void MPI_group_ring_create(MPI_Comm old_comm, int nodes,
    int index[], MPI_Comm* new_comm) {

    int size, rank;

    MPI_Comm_size(old_comm, &size);
    MPI_Comm_rank(old_comm, &rank);

    MPI_Group old_group;
    MPI_Comm_group(old_comm, &old_group);

    MPI_Group ring_group;
    MPI_Group_incl(old_group, nodes, index, &ring_group);

    MPI_Comm ring_comm;
    MPI_Comm_create(old_comm, ring_group, &ring_comm);

    MPI_Group_free(&old_group);

    *new_comm = ring_comm;
}


list<int> Dejkstra(int n, int ST, int dest)
{

    vector<vector<int>> matrix(n);
    for (int i = 0; i < n; i++)
        matrix[i].resize(n);

    for (int i = 0; i < n - 1; i++) {
        matrix[i][i + 1] = matrix[i + 1][i] = 1;
    }
    matrix[0][n - 1] = matrix[n - 1][0] = 1;

    int infin = 1000000;

    vector <int> d(n, infin);
    vector <bool> s(n, false);
    vector <list<int>> path(n);

    d[ST] = 0;
    s[ST] = true;

    int o = ST;

    for (int j = 0; j < n - 1; j++) {

        for (int i = 0; i < n; i++) {
            if (s[i] == false && matrix[o][i] != 0 && d[o] + matrix[o][i] < d[i]) {
                d[i] = d[o] + matrix[o][i];
                path[i] = path[o];
                path[i].push_back(i);
            }
        }


        int min = infin;
        for (int i = 0; i < n; i++) {
            if (s[i] == false && d[i] < min) {
                min = d[i];
                o = i;
            }
        }

        s[o] = true;

    }
    return path[dest];
}


int Ring_Send(const void* buf, int count, MPI_Datatype datatype,
    int from, int dest, int tag, MPI_Comm comm) {
    if (comm != MPI_COMM_NULL) {
        int size, rank;

        MPI_Comm_rank(comm, &rank);
        MPI_Comm_size(comm, &size);

        if (rank == from) {
            
            list<int> t = Dejkstra(size, from, dest);
            vector<int> routeTwo(size, -1);
            auto it = t.begin();
            auto at = ++t.begin();
            routeTwo[from] = *it;
            for (;at != t.end(); it++, at++) {
                routeTwo[*it] = *at;
            }

            for (int i = 0; i < size; i++) {
                if (i != from) {
                    MPI_Send(&routeTwo[i], 1, MPI_INT, i, tag, comm);
                }
            }
            MPI_Send(buf, count, datatype, routeTwo[rank], tag, comm);

            return 0;
        }
        else {
            MPI_Status status;
            int d;
            MPI_Recv(&d, 1, MPI_INT, from, MPI_ANY_TAG, comm, &status);

            if (d != -1) {
                int size_of_datatype;
                MPI_Type_size(datatype, &size_of_datatype);
                std::vector<char> local_vec(size_of_datatype * count);
                MPI_Recv(local_vec.data(), count, datatype, MPI_ANY_SOURCE, tag, comm, &status);
                MPI_Send(local_vec.data(), count, datatype, d, tag, comm);
            }
            return 0;
        }
        return 0;
    }
        
    else {
        return -1;
    }
    return 0;
}

