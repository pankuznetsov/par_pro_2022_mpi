// Copyright 2022 Sukharev Artem
#ifndef MODULES_TASK_3_SUKHAREV_A_GSA_PARALL_BY_CHARACT_GLOB_OPT_H_
#define MODULES_TASK_3_SUKHAREV_A_GSA_PARALL_BY_CHARACT_GLOB_OPT_H_

#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>

struct test_point {
  std::vector<double> x;
  double z;
  explicit test_point(const std::vector<double>& _x = {0, 0},
                      const double& _z = 0)
      : x(_x), z(_z) {}
  bool operator<(const test_point& a) const {
    if (x != a.x) return x < a.x;
    return z < a.z;
  }
  double operator[](const int a) const { return x[a]; }
};

test_point one_dimension_gsa(std::function<double(double, double)> func,
                             double x0, double leftBorder, double rightBorder,
                             double deltaX);

double two_dimension_gsa(std::function<double(double, double)> func,
                         double leftBorderX, double rightBorderX,
                         double leftBorderY, double rightBorderY,
                         double deltaXY);

double GSA(std::function<double(double, double)> func, double leftBorderX = 0.0,
           double rightBorderX = 1.0, double leftBorderY = 0.0,
           double rightBorderY = 1.0, double deltaXY = 0.001);

#endif  // MODULES_TASK_3_SUKHAREV_A_GSA_PARALL_BY_CHARACT_GLOB_OPT_H_
