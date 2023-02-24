// Copyright 2022 Yarakhtin Anton
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_3/yarakhtin_a_cannons_algorithm/cannons_algorithm.h"

Matrix::Matrix(int x_size, int y_size)
    : x_size(x_size), y_size(y_size) {
    if (x_size > 0 && y_size > 0) {
        data = new double[x_size * y_size];
    }
}

Matrix::Matrix(const Matrix& m) {
    x_size = m.x_size;
    y_size = m.y_size;
    data = new double[x_size * y_size];
    for (int i = 0; i < x_size * y_size; i++) {
        data[i] = m.data[i];
    }
}

Matrix::~Matrix() {
    delete[] data;
}

Matrix& Matrix::operator=(const Matrix& m) {
    if (this == &m) {
        return *this;
    }
    delete[] data;

    x_size = m.x_size;
    y_size = m.y_size;
    data = new double[x_size * y_size];
    for (int i = 0; i < x_size * y_size; i++) {
        data[i] = m.data[i];
    }
    return *this;
}

Matrix generateMatrix(int x_size, int y_size) {
    Matrix result(x_size, y_size);
    std::random_device dev;
    std::mt19937 gen(dev());
    for (int i = 0; i < x_size * y_size; i++) {
        result.data[i] = 0.01 * (gen() % 10000);
    }
    return result;
}

Matrix multiplySequentially(const Matrix& A, const Matrix& B) {
    Matrix result(B.x_size, A.y_size);
    for (int i = 0; i < result.y_size; i++) {
        for (int j = 0; j < result.x_size; j++) {
            result.data[i * result.x_size + j] = 0;
            for (int k = 0; k < A.x_size; k++) {
                result.data[i * result.x_size + j] += A.data[i * A.x_size + k] * B.data[k * B.x_size + j];
            }
        }
    }
    return result;
}

Matrix sumSequentially(const Matrix& A, const Matrix& B) {
    Matrix result(A.x_size, A.y_size);
    for (int i = 0; i < result.y_size; i++) {
        for (int j = 0; j < result.x_size; j++) {
            result.data[i * result.x_size + j] = A.data[i * A.x_size + j] + B.data[i * B.x_size + j];
        }
    }
    return result;
}

static void calcProcCount(int* size, int y_len, int* dim) {
    int result = sqrt(*size);
    while (y_len % result != 0) {
        result--;
    }
    *dim = result;
    *size = result * result;
}

Matrix multiplyParallel(const Matrix& A, const Matrix& B) {
    int rank;
    int size;
    MPI_Status status;
    MPI_Comm cm;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (A.x_size != B.y_size) {
        return Matrix(0, 0);
    }

    int x_size = B.x_size;
    int y_size = A.y_size;

    MPI_Bcast(&x_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&y_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int dim;
    calcProcCount(&size, y_size, &dim);

    if (rank >= size) {
        return Matrix(0, 0);
    }

    if (size == 1) {
        return multiplySequentially(A, B);
    }

    const int dimensions[2] = { dim, dim };
    const int periods[2] = { 1, 1 };

    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, periods, 0, &cm);

    int portion = y_size / dim;
    Matrix local_A(portion, portion);
    Matrix local_B(portion, portion);
    Matrix local_C(portion, portion);

    for (int i = 0; i < portion * portion; i++) {
        local_C.data[i] = 0;
    }

    if (rank > 0) {
        MPI_Recv(local_A.data, portion * portion, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(local_B.data, portion * portion, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    } else {
        int coords[2];
        for (int i = 1; i < size; i++) {
            MPI_Cart_coords(cm, i, 2, coords);
            int B_x = (coords[1] + coords[0]) % dim;
            int B_y = coords[1];
            int A_x = coords[0];
            int A_y = (coords[0] + coords[1]) % dim;
            for (int j = 0; j < portion; j++) {
                for (int k = 0; k < portion; k++) {
                    int A_j = A_x * portion + j;
                    int A_k = A_y * portion + k;
                    int B_j = B_x * portion + j;
                    int B_k = B_y * portion + k;
                    local_A.data[j * local_A.x_size + k] = A.data[A_j * A.x_size + A_k];
                    local_B.data[j * local_B.x_size + k] = B.data[B_j * B.x_size + B_k];
                }
            }
            MPI_Send(local_A.data, portion * portion, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(local_B.data, portion * portion, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
        for (int j = 0; j < portion; j++) {
            for (int k = 0; k < portion; k++) {
                local_A.data[j * local_A.x_size + k] = A.data[j * A.x_size + k];
                local_B.data[j * local_B.x_size + k] = B.data[j * B.x_size + k];
            }
        }
    }

    for (int i = 0; i < dim; i++) {
        auto M = multiplySequentially(local_A, local_B);
        local_C = sumSequentially(local_C, M);

        int source_x;
        int dest_x;
        int source_y;
        int dest_y;

        MPI_Cart_shift(cm, 1, 1, &source_x, &dest_x);
        MPI_Sendrecv_replace(local_A.data, portion * portion, MPI_DOUBLE, source_x, 0, dest_x, 0, cm, &status);

        MPI_Cart_shift(cm, 0, 1, &source_y, &dest_y);
        MPI_Sendrecv_replace(local_B.data, portion * portion, MPI_DOUBLE, source_y, 0, dest_y, 0, cm, &status);
    }

    if (rank > 0) {
        MPI_Send(local_C.data, portion * portion, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        return Matrix(0, 0);
    } else {
        Matrix result(x_size, y_size);

        for (int i = 0; i < portion; i++) {
            for (int j = 0; j < portion; j++) {
                result.data[i * result.x_size + j] = local_C.data[i * local_C.x_size + j];
            }
        }
        for (int i = 1; i < size; i++) {
            int rcoords[2];
            MPI_Cart_coords(cm, i, 2, rcoords);

            MPI_Recv(local_C.data, portion * portion, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
            for (int j = 0; j < portion; j++) {
                for (int k = 0; k < portion; k++) {
                    int r_j = rcoords[0] * portion + j;
                    int r_k = rcoords[1] * portion + k;
                    result.data[r_j * result.x_size + r_k] = local_C.data[j * local_C.x_size + k];
                }
            }
        }

        return result;
    }
}
