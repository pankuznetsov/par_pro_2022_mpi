// Copyright 2022 Reshetnik Yana
#include <mpi.h>
#include <cmath>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_3/reshetnik_y_matr_mult_cannons/matr_mult_cannons.h"

void fill_matrix(double* m, int columns, int rows) {
    std::random_device device;
    std::mt19937 mtgen(device());
    for (int i = 0; i < columns * rows; i++)
        m[i] = mtgen() % 100;
}

double& get(double* m, int x, int y, int columns) {
    return m[y * columns + x];
}

void multiply_matrix(double* result, double* m1, int columns1, int rows1, double* m2, int columns2, int rows2) {
    for (int i = 0; i < rows1; i++)
        for (int j = 0; j < columns2; j++)
            get(result, j, i, columns2) = 0;
    for (int i = 0; i < rows1; i++)
        for (int j = 0; j < columns2; j++)
            for (int k = 0; k < columns1; k++)
                get(result, j, i, columns2) += get(m1, k, i, columns1) * get(m2, j, k, columns2);
}

void cannon_parallel_multiply(double* result, double* m1, int columns1, int rows1,
                                double* m2, int columns2, int rows2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (columns1 != rows2)
        return;

    int c, r;
    if (rank == 0) {
        c = columns2;
        r = rows1;
    }

    MPI_Bcast(&c, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&r, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    for (size = sqrt(size); r % size != 0; size--)
        continue;

    if (rank >= size * size)
        return;

    if (size == 1)
        return multiply_matrix(result, m1, columns1, rows1, m2, columns2, rows2);

    MPI_Comm comm;
    int* dr = new int[4] {size, size, 1, 1};
    MPI_Cart_create(MPI_COMM_WORLD, 2, dr, dr + 2, 0, &comm);

    int fraction = r / size;
    double* a = new double[fraction * fraction];
    double* b = new double[fraction * fraction];
    double* m = new double[fraction * fraction];

    for (int i = 0; i < fraction * fraction; i++) {
        a[i] = 0;
        b[i] = 0;
        m[i] = 0;
    }

    // Step 1

    MPI_Status status;
    if (rank == 0) {
        for (int i = 1; i < size * size; i++) {
            int ccoords[3];
            MPI_Cart_coords(comm, i, 2, ccoords);
            ccoords[2] = (ccoords[0] + ccoords[1]) % size;
            for (int j = 0; j < fraction; j++) {
                for (int k = 0; k < fraction; k++) {
                    get(a, k, j, fraction) = get(m1, ccoords[2] * fraction + k, ccoords[0] * fraction + j, columns1);
                    get(b, k, j, fraction) = get(m2, ccoords[1] * fraction + k, ccoords[2] * fraction + j, columns2);
                }
            }
            MPI_Send(a, fraction * fraction, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(b, fraction * fraction, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
        for (int i = 0; i < fraction; i++) {
            for (int j = 0; j < fraction; j++) {
                get(a, j, i, fraction) = get(m1, j, i, columns1);
                get(b, j, i, fraction) = get(m2, j, i, columns2);
            }
        }
    } else {
        MPI_Recv(a, fraction * fraction, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(b, fraction * fraction, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    }

    // Step 2

    double* temp = new double[fraction * fraction];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < fraction * fraction; j++)
            temp[j] = 0;
        multiply_matrix(temp, a, fraction, fraction, b, fraction, fraction);
        for (int j = 0; j < fraction * fraction; j++)
            m[j] += temp[j];

        int sd[2];

        MPI_Cart_shift(comm, 1, 1, sd, sd + 1);
        MPI_Sendrecv_replace(a, fraction * fraction, MPI_DOUBLE, sd[0], 0, sd[1], 0, comm, &status);

        MPI_Cart_shift(comm, 0, 1, sd, sd + 1);
        MPI_Sendrecv_replace(b, fraction * fraction, MPI_DOUBLE, sd[0], 0, sd[1], 0, comm, &status);
    }
    delete[] temp;

    // Step 3

    if (rank != 0) {
        MPI_Send(m, fraction * fraction, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        return;
    }

    for (int i = 0; i < fraction; i++)
        for (int j = 0; j < fraction; j++)
            get(result, j, i, c) = get(m, j, i, fraction);

    for (int i = 1; i < size * size; i++) {
        MPI_Recv(m, fraction * fraction, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

        int pc[2];
        MPI_Cart_coords(comm, status.MPI_SOURCE, 2, pc);

        for (int j = 0; j < fraction; j++)
            for (int k = 0; k < fraction; k++)
                get(result, pc[1] * fraction + k, pc[0] * fraction + j, c)
                    = get(m, k, j, fraction);
    }
}
