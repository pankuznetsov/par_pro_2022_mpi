// Copyright 2022 Sukharev Artem
#include <gtest/gtest.h>
#include <mpi.h>

#include <functional>
#include <iostream>
#include <cmath>

#include <gtest-mpi-listener.hpp>
#include "./glob_opt.h"

constexpr double SIGMA = 0.01;

TEST(Glob_opt_parall_by_characteristics, Test_Independent_Coordinates) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::function<double(double, double)> func = [](double x,
                                                  double y) -> double {
    return std::sin(x) + std::sin(y * 20 + 2) / y -
           5 * y * std::cos(3 * y + 10);
  };
  double lbX = -3, rbX = 3;
  double lbY = 0.25, rbY = 2.5;
  double errorXY = 0.01;
  double expectedRes = -6.2160;
  if (rank == 0) {
    double result = GSA(func, lbX, rbX, lbY, rbY, errorXY);
    // std::cout << result << "\n";
    ASSERT_TRUE(std::abs(result - expectedRes) < SIGMA);
  } else {
    GSA(func, lbX, rbX, lbY, rbY, errorXY);
  }
}

TEST(Glob_opt_parall_by_characteristics, Test_One_Extremum_Function) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::function<double(double, double)> func = [](double x,
                                                  double y) -> double {
    return (std::pow(x, 2) + std::pow(y, 2)) - 4.5;
  };
  double lbX = -2, rbX = 2;
  double lbY = -2, rbY = 2;
  double errorXY = 0.01;
  double expectedRes = -4.5000;
  if (rank == 0) {
    double result = GSA(func, lbX, rbX, lbY, rbY, errorXY);
    // std::cout << result << "\n";
    ASSERT_TRUE(std::abs(result - expectedRes) < SIGMA);
  } else {
    GSA(func, lbX, rbX, lbY, rbY, errorXY);
  }
}

TEST(Glob_opt_parall_by_characteristics, Test_Multi_Extremum_Function1) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::function<double(double, double)> func = [](double x,
                                                  double y) -> double {
    return std::sin(3 * x + 2 * std::cos(y)) +
           std::cos(std::pow((x + 1), 2) - 2 * y);
  };
  double lbX = -4, rbX = 2;
  double lbY = -2, rbY = 3;
  double errorXY = 0.01;
  double expectedRes = -2.0000;
  if (rank == 0) {
    double result = GSA(func, lbX, rbX, lbY, rbY, errorXY);
    // std::cout << result << "\n";
    ASSERT_TRUE(std::abs(result - expectedRes) < SIGMA);
  } else {
    GSA(func, lbX, rbX, lbY, rbY, errorXY);
  }
}

TEST(Glob_opt_parall_by_characteristics, Test_Multi_Extremum_Function2) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::function<double(double, double)> func = [](double x,
                                                  double y) -> double {
    return (y - 1) * (y + 1) *
           (3 * std::sin(-x * 2) - x * std::cos(2 * x) - 2 * std::sin(5 * x));
  };
  double lbX = -1, rbX = 2.5;
  double lbY = -1.5, rbY = 1.5;
  double errorXY = 0.01;
  double expectedRes = -5.5625;
  if (rank == 0) {
    double result = GSA(func, lbX, rbX, lbY, rbY, errorXY);
    // std::cout << result << "\n";
    ASSERT_TRUE(std::abs(result - expectedRes) < SIGMA);
  } else {
    GSA(func, lbX, rbX, lbY, rbY, errorXY);
  }
}

TEST(Glob_opt_parall_by_characteristics, Test_One_Dimension) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::function<double(double, double)> func = [](double x,
                                                  double y) -> double {
    return y * std::sin(2 * y + 2) - std::cos(2 * y) + 10;
  };
  double lbX = 10, rbX = 10;
  double lbY = -3, rbY = 10;
  double errorXY = 0.01;
  double expectedRes = 3.2694;
  if (rank == 0) {
    double result = GSA(func, lbX, rbX, lbY, rbY, errorXY);
    // std::cout << result << "\n";
    ASSERT_TRUE(std::abs(result - expectedRes) < SIGMA);
  } else {
    GSA(func, lbX, rbX, lbY, rbY, errorXY);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
      ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
