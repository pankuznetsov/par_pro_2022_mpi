// Copyright 2022 Zagrebin S.

#include <mpi.h>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "../../../modules/task_3/zagrebin_s_dijkstra/dijkstra.h"

#define INF std::numeric_limits<int>::max()

Random::Random(double q) {
    if ((q < 0) || (q > 1))
        throw - 1;
    coef = RAND_MAX * q;
}

void Random::Rand_Init(int seed) {
    if (seed == -1)
        srand(time(NULL));
    else
        srand(seed);
}

inline bool Random::toss() noexcept {
    return rand() <= coef;
}

int Random::give(unsigned i) noexcept {
    return rand() % i;
}


void GenSymMatr(int** m, int n, double p, int max_w) {
  Random r(p);
  *m = new int[n*n];
  for (int i = 0; i < n; ++i)
    (*m)[i*n+i] = 0;

  for (int i = 0; i < n-1; ++i)
    for (int j = i+1; j < n; ++j)
      if (r.toss())
        (*m)[j*n+i] = (*m)[i*n+j] = Random::give(max_w-1)+1;
      else
        (*m)[j*n+i] = (*m)[i*n+j] = 0;
}


V findMin(int* arr, int n) {
    V min = V{INF, -1};
    for (int i = 0; i < n; ++i)
        if (arr[i] != 0 &&
            (min.ind == -1 || arr[i] < min.d))
            min = V{arr[i], i};
    return min;
}


void Dijkstra(int* m, int n, int source, int* res) {
    int rank, size, color, d, r;
    int *w;
    MPI_Comm resComm, dComm;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    color = rank > n - 1 ? 0 : 1;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &resComm);
    if (color == 0)
        return;

    MPI_Comm_size(resComm, &size);
    d = n / size; r = n - d * (size - 1);

    color = (rank == size - 1) ? 0 : 1;
    MPI_Comm_split(resComm, color, rank, &dComm);

    if (size != 1)
        w = new int[rank == size - 1 ? n*r : n*d];

    if (color != 0)
        MPI_Scatter(m, n * d, MPI_INT,
                    w, n * d, MPI_INT,  0, dComm);

    if (size != 1) {
        if (rank == size - 1) {
            MPI_Status status;
            MPI_Recv(w, n * r, MPI_INT, 0, MPI_ANY_TAG, resComm, &status);
        }
        if (rank == 0)
            MPI_Send(m+n*(n-r), n * r, MPI_INT, size-1, 0, resComm);
    } else {
        w = m;
    }

    int s = (rank == size - 1) ? r : d;
    int *locMin = new int[s];
    for (int i = 0; i < s; ++i)
        locMin[i] = INF;

    V choice = {0, source};
    if (rank == 0) {
        for (int i = 0; i < n; ++i)
            res[i] = 0;
    }

    while (true) {
        if (choice.d == INF)
            break;
        int tmp = choice.ind - rank * d;
        if (tmp >= 0 && tmp < s) {
            locMin[tmp] = 0;
        }

        for (int i = 0; i < s; ++i)
            if ( w[i*n+choice.ind] != 0 &&
                    (choice.d + w[i*n+choice.ind] < locMin[i]) )
                locMin[i] = choice.d + w[i*n+choice.ind];

        V min = findMin(locMin, s);
        min.ind += rank * d;

        MPI_Reduce(&min, &choice, 1, MPI_2INT,
                    MPI_MINLOC, 0, resComm);
        MPI_Bcast(&choice, 1, MPI_2INT, 0, resComm);
        if (rank == 0) {
            if (choice.d != INF)
                res[choice.ind] = choice.d;
        }
    }
    delete[] locMin;
    if (size != 1)
        delete[] w;
}
