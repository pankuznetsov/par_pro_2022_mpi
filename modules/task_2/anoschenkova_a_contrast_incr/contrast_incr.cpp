// Copyright 2023 Anoschenkova Anna
#include "../../../modules/task_2/anoschenkova_a_contrast_incr/contrast_incr.h"
#include <mpi.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>

std::vector<int> genImage(int rows, int cols) {
    std::vector<int> image(rows * cols);
    std::random_device dev;
    std::mt19937 random(dev());

    for (int i = 0; i < rows * cols; i++) image[i] = random() % 256;
    return image;
}

std::vector<int> incrContrastParallel(const std::vector<int>& img, int rows,
                                      int cols, int correct) {
    if (static_cast<int>(img.size()) != rows * cols || correct < 1) {
        throw -1;
    }

    int csize, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &csize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int amount = rows / csize;
    int rest = rows % csize;
    std::vector<int> res(rows * cols);
    std::vector<int> rank_v((amount + 1) * cols);

    int middle = 0;
    int mid = 0;
    if (rank == 0) {
        if (amount > 0) {
            for (int proc = 1; proc < csize; proc++) {
                if (proc < rest) {
                    MPI_Send(&img[(amount * proc + proc) * cols],
                             (amount + 1) * cols, MPI_INT, proc, 1,
                             MPI_COMM_WORLD);
                } else {
                    MPI_Send(&img[(amount * proc + rest) * cols], amount * cols,
                             MPI_INT, proc, 2, MPI_COMM_WORLD);
                }
            }
        }
    } else if (amount > 0) {
        if (rank < rest) {
            MPI_Status stat;
            MPI_Recv(&rank_v[0], (amount + 1) * cols, MPI_INT, 0, 1,
                     MPI_COMM_WORLD, &stat);
            amount++;
        } else {
            MPI_Status stat;
            MPI_Recv(&rank_v[0], amount * cols, MPI_INT, 0, 2, MPI_COMM_WORLD,
                     &stat);
        }
    }
    int rest1 = rest;
    if (rank == 0) {
        MPI_Status status;
        if (amount > 0) {
            if (rest > 0) {
                rest1 = 1;
            } else {
                rest1 = 0;
            }
        }
        rank_v = std::vector<int>(img.begin(),
                                  img.begin() + (rest1 + amount) * cols);
        for (int i = 0; i < (rest1 + amount) * cols; i++) {
            middle += rank_v[i];
        }
        if (amount > 0) {
            for (int proc = 1; proc < csize; proc++) {
                MPI_Recv(&mid, 1, MPI_INT, proc, 3, MPI_COMM_WORLD, &status);
                middle += mid;
            }
        }
        middle = middle / (cols * rows);
    } else if (amount > 0) {
        for (int i = 0; i < amount * cols; i++) {
            middle += rank_v[i];
        }
        MPI_Send(&middle, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
    }

    MPI_Bcast(&middle, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int _amount = 256 / csize;
    int _rest = 256 % csize;
    std::vector<int> palette(256);

    float corr = 1.0 + static_cast<float>(correct) / 100;
    std::vector<int> a(_amount);
    if (rank == 0) {
        MPI_Status status;
        for (int i = 0; i < _amount + _rest; i++) {
            int delta = i - middle;
            palette[i] = middle + corr * delta;
            if (palette[i] > 255) {
                palette[i] = 255;
            } else if (palette[i] < 0) {
                palette[i] = 0;
            }
        }
        if (_amount > 0) {
            for (int proc = 1; proc < csize; proc++) {
                MPI_Recv(&palette[_amount + _rest + _amount * (proc - 1)],
                         _amount, MPI_INT, proc, 3, MPI_COMM_WORLD, &status);
            }
        }
    } else if (_amount > 0) {
        for (int i = 0; i < _amount; i++) {
            int delta = i + _amount + _rest + _amount * (rank - 1) - middle;
            a[i] = middle + corr * delta;
            if (a[i] > 255) {
                a[i] = 255;
            }
            if (a[i] < 0) {
                a[i] = 0;
            }
        }

        MPI_Send(&a[0], _amount, MPI_INT, 0, 3, MPI_COMM_WORLD);
    }
    MPI_Bcast(&palette[0], 256, MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<int> b(amount * cols);
    if (rank == 0) {
        MPI_Status status;
        for (int i = 0; i < (amount + rest1) * cols; i++)
            res[i] = palette[img[i]];
        if (amount > 0) {
            for (int proc = 1; proc < csize; proc++) {
                if (proc < rest) {
                    MPI_Recv(&res[(amount * proc + proc) * cols],
                             (amount + 1) * cols, MPI_INT, proc, 4,
                             MPI_COMM_WORLD, &status);
                } else {
                    MPI_Recv(&res[(amount * proc + rest) * cols], amount * cols,
                             MPI_INT, proc, 4, MPI_COMM_WORLD, &status);
                }
            }
        }
    } else if (amount > 0) {
        for (int i = 0; i < amount * cols; i++) b[i] = palette[rank_v[i]];
        MPI_Send(&b[0], amount * cols, MPI_INT, 0, 4, MPI_COMM_WORLD);
    }
    return res;
}

std::vector<int> incrContrast(const std::vector<int>& img, int rows, int cols,
                              int correct) {
    if (static_cast<int>(img.size()) != rows * cols || correct < 1) throw -1;
    std::vector<int> res(rows * cols);

    int middle = 0;
    for (int i = 0; i < rows * cols; i++) middle += img[i];

    middle = middle / (rows * cols);
    float corr = 1.0 + static_cast<float>(correct) / 100;
    std::vector<int> palette(256);

    for (int i = 0; i < 256; i++) {
        int delta = i - middle;
        palette[i] = middle + corr * delta;
        if (palette[i] > 255) {
            palette[i] = 255;
        }
        if (palette[i] < 0) {
            palette[i] = 0;
        }
    }

    for (int i = 0; i < rows * cols; i++) {
        res[i] = palette[img[i]];
    }

    return res;
}
