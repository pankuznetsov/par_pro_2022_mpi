// Copyright 2022 Kuznetsov Sergey
#ifndef DOCUMENTS_MPI_TASK3_MODULES_TASK_3_KUZNETSOV_S_2D_INTEGRATION_TRAPEZZOID_INTEGRATION2D_TRAPEZZOID_H_
#define DOCUMENTS_MPI_TASK3_MODULES_TASK_3_KUZNETSOV_S_2D_INTEGRATION_TRAPEZZOID_INTEGRATION2D_TRAPEZZOID_H_

#include <stdio.h>
#include <mpi.h>
#include <iostream>

template <typename F>
double Integrate2D_Rectangles(double a, double b, double c, double d, int nx,
                              int ny, F f) {
  double sum = 0;
  double hx = (bx - ax) / static_cast<double>(nx),
         hy = (by - ay) / static_cast<double>(ny);
  for (int sy = 0; sy < ny; sy++) {
    for (int sx = 0; sx < nx; sx++) {
      sum += f(ax + hx * sx, ay + hy * sy) * (hx * hy);
    }
  }
  return sum;
}

template <typename F>
double Integrate2D_Trapezoid(double ax, double bx, double ay, double by, int nx,
                             int ny, F f) {
  double sum = 0;
  double hx = (bx - ax) / static_cast<double>(nx),
         hy = (by - ay) / static_cast<double>(ny);
  for (int sy = 0; sy < ny; sy++) {
    for (int sx = 0; sx < nx; sx++) {
      sum +=
          (f(ax + hx * sx, ay + hy * sy) + f(ax + hx * (sx + 1), ay + hy * sy) +
           f(ax + hx * sx, ay + hy * (sy + 1)) +
           f(ax + hx * (sx + 1), ay + hy * (sy + 1))) *
          (hx * hy) / 4;
    }
  }
  return sum;
}

template <typename F>
double Integrate2D_Trapezoid_Parallel(double ax, double bx, double ay,
                                      double by, int nx, int ny, F f) {
  int np, pid;  // np - number of the processes,  pid - it's identifier
  MPI_Status status;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  // master process
  double totalSum = 0;
  if (pid == 0) {
    double tmp;
    for (int i = 1; i < np; i++) {
      MPI_Recv(&tmp, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
      totalSum += tmp;
    }
    totalSum += Integrate2D_Trapezoid(ax + ((bx - ax) / np) * pid,
                                      ax + ((bx - ax) / np) * (pid + 1), ay, by,
                                      nx / np, ny, f);
  } else {
    double p_sum = Integrate2D_Trapezoid(ax + ((bx - ax) / np) * pid,
                                         ax + ((bx - ax) / np) * (pid + 1), ay,
                                         by, nx / np, ny, f);
    MPI_Send(&p_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }
  return totalSum;
}
#endif  // DOCUMENTS_MPI_TASK3_MODULES_TASK_3_KUZNETSOV_S_2D_INTEGRATION_TRAPEZZOID_INTEGRATION2D_TRAPEZZOID_H_
