// Copyright 2022 Sukharev Artem
#include "../../modules/task_3/sukharev_a_gsa_parall_by_charact/glob_opt.h"

#include <mpi.h>

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include <cmath>

constexpr double R = 9;
constexpr int K = 200;

void one_test(std::vector<double> x, std::function<double(double, double)> func,
              std::vector<test_point>* points, test_point* best) {
  double z = func(x[0], x[1]);
  points->push_back(test_point(x, z));
  if (best->z > z) {
    *best = test_point(x, z);
  }
}

double calc_const_M(const std::vector<test_point>& points, int dimension) {
  double miniM = 0;
  for (int i = 1; i < points.size(); i++) {
    miniM =
        std::max(miniM, std::abs(points[i].z - points[i - 1].z) /
                            (points[i][dimension] - points[i - 1][dimension]));
  }
  double M = 1;
  if (miniM > 0) {
    M = R * miniM;
  }
  return M;
}

std::vector<std::pair<double, int>> calc_characteristics(
    const std::vector<test_point>& points, double M, int dimension) {
  std::vector<std::pair<double, int>> characteristics(points.size() - 1);
  for (int i = 0; i < points.size() - 1; i++) {
    double dX = points[i + 1][dimension] - points[i][dimension];
    double dZ = points[i + 1].z - points[i].z;
    double sZ = points[i + 1].z + points[i].z;
    characteristics[i] = {M * dX + dZ * dZ / (M * dX) - 2 * sZ, i};
  }
  return characteristics;
}

test_point one_dimension_gsa(std::function<double(double, double)> func,
                             double x0, double leftBorder, double rightBorder,
                             double deltaX) {
  std::vector<test_point> points;
  test_point best;
  {
    one_test(std::vector<double>{x0, leftBorder}, func, &points, &best);
    best = points[0];
    one_test(std::vector<double>{x0, rightBorder}, func, &points, &best);
  }
  for (int j = 2; j < K; j++) {
    sort(points.begin(), points.end());
    double M = calc_const_M(points, 1);
    std::vector<std::pair<double, int>> characteristics =
        calc_characteristics(points, M, 1);
    sort(characteristics.rbegin(), characteristics.rend());
    int tmp_interval = characteristics[0].second;
    if (points[tmp_interval + 1][1] - points[tmp_interval][1] < deltaX) {
      break;
    }
    double newX1 =
        (points[tmp_interval + 1][1] + points[tmp_interval][1]) / 2 -
        (points[tmp_interval + 1].z - points[tmp_interval].z) / (2 * M);
    one_test(std::vector<double>{x0, newX1}, func, &points, &best);
  }
  return best;
}

void do_tests(const std::vector<double>& toDo,
              std::function<double(double, double)> func,
              std::vector<test_point>* points, test_point* best,
              double leftBorderY, double rightBorderY, double deltaXY) {
  int procNum = toDo.size();
  test_point bestOneDim;
  bestOneDim =
      one_dimension_gsa(func, toDo[0], leftBorderY, rightBorderY, deltaXY);
  if (bestOneDim.z < best->z) {
    *best = bestOneDim;
  }
  points->push_back(bestOneDim);
  // std::cout<< bestOneDim[0] <<"::"<<bestOneDim[1]<<"=="<<bestOneDim.z<<"\n";
  std::vector<double> results(3 * procNum);
  int fl = 1;
  MPI_Bcast(&fl, 1, MPI_INT, 0, MPI_COMM_WORLD);
  double tmp = 0;
  MPI_Scatter(toDo.data(), 1, MPI_DOUBLE, &tmp, 1, MPI_DOUBLE, 0,
              MPI_COMM_WORLD);
  std::vector<double> _{0, 0, 0};

  MPI_Gather(_.data(), 3, MPI_DOUBLE, results.data(), 3, MPI_DOUBLE, 0,
             MPI_COMM_WORLD);
  for (int i = 1; i < procNum; i++) {
    bestOneDim =
        test_point(std::vector<double>{results[i * 3], results[i * 3 + 1]},
                   results[i * 3 + 2]);
    if (bestOneDim.z < best->z) {
      *best = bestOneDim;
    }
    points->push_back(bestOneDim);
    // std::cout<< bestOneDim[0]<<"::"<<bestOneDim[1]<<"=="<<bestOneDim.z<<"\n";
  }
}

double two_dimension_gsa(std::function<double(double, double)> func,
                         double leftBorderX, double rightBorderX,
                         double leftBorderY, double rightBorderY,
                         double deltaXY) {
  int procNum;
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  std::vector<test_point> points;
  test_point best;
  {
    test_point tmp = one_dimension_gsa(func, rightBorderX, leftBorderY,
                                       rightBorderY, deltaXY);
    best = tmp;
    points.push_back(tmp);
    std::vector<double> toDo;
    for (int i = 0; i < procNum; i++) {
      toDo.push_back(leftBorderX + i * (rightBorderX - leftBorderX) / procNum);
    }
    do_tests(toDo, func, &points, &best, leftBorderY, rightBorderY, deltaXY);
  }
  for (; points.size() < K;) {
    sort(points.begin(), points.end());
    double M = calc_const_M(points, 0);
    std::vector<std::pair<double, int>> characteristics =
        calc_characteristics(points, M, 0);
    sort(characteristics.rbegin(), characteristics.rend());
    std::vector<double> toDo;

    for (int i = 0; i < procNum; i++) {
      int tmp_interval = characteristics[i].second;
      if (points[tmp_interval + 1][0] - points[tmp_interval][0] < deltaXY) {
        return best.z;
      }

      double newX0 =
          (points[tmp_interval + 1][0] + points[tmp_interval][0]) / 2 -
          (points[tmp_interval + 1].z - points[tmp_interval].z) / (2 * M);
      toDo.push_back(newX0);
    }

    do_tests(toDo, func, &points, &best, leftBorderY, rightBorderY, deltaXY);
  }
  return best.z;
}

double GSA(std::function<double(double, double)> func, double leftBorderX,
           double rightBorderX, double leftBorderY, double rightBorderY,
           double deltaXY) {
  int rank, procNum;
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    double result = two_dimension_gsa(func, leftBorderX, rightBorderX,
                                      leftBorderY, rightBorderY, deltaXY);
    int fl = 0;
    MPI_Bcast(&fl, 1, MPI_INT, 0, MPI_COMM_WORLD);
    return result;
  }
  while (1) {
    int fl = 1;
    MPI_Bcast(&fl, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (fl == 0) {
      break;
    }
    double x0 = 0;
    MPI_Scatter(&x0, 1, MPI_DOUBLE, &x0, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    test_point bestOneDim;
    bestOneDim =
        one_dimension_gsa(func, x0, leftBorderY, rightBorderY, deltaXY);
    std::vector<double> tmp{bestOneDim[0], bestOneDim[1], bestOneDim.z};
    MPI_Gather(tmp.data(), 3, MPI_DOUBLE, tmp.data(), 3, MPI_DOUBLE, 0,
               MPI_COMM_WORLD);
  }
  return 0;
}
