// Copyright 2023 Raschetnov Alexei
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/raschetnov_a_rows_sum/rows_sum.h"

std::vector<int> getRandomMatrix(int rows, int columns) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::vector<int> matrix(rows * columns);
  for (int i = 0; i < rows * columns; i++) { matrix[i] = gen() % 100; }
  return matrix;
}

std::vector<int> getSequentialSum(const std::vector<int>& local_matrix, int rows, int columns) {
  std::vector<int> result(rows);
  for (int i = 0; i < rows; i++) {
    int sum = 0;
    for (int j = 0; j < columns; j++)
      sum += local_matrix[j + i * columns];
    result[i] = sum;
  }
  return result;
}

std::vector<int> getParallelSum(const std::vector<int>& global_matrix, int rows, int columns) {
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> result(rows);
  std::vector<int> tmp;
  std::vector<int> rule(size);
  std::vector<int> sendcounts(size);
  std::vector<int> displs(size);
  std::vector<int> recv_displs(size);
  std::vector<int> local_matrix(rows * columns);

  if (rank == 0) {
    for (int i = 0; i < rows; i++)
      rule[i % size]++;
    displs[0] = 0;
    for (int i = 1; i < size; i++) {
      if (i >= rows)
        displs[i] = 0;
      else
        displs[i] = displs[i - 1] + rule[i - 1] * columns;
    }
    recv_displs[0] = 0;
    for (int i = 1; i < size; i++)
      recv_displs[i] = recv_displs[i - 1] + rule[i - 1];
    for (int i = 0; i < size; i++) {
      if (i >= rows)
        sendcounts[i] = 1;
      else
        sendcounts[i] = rule[i] * columns;
    }
  }

  MPI_Scatterv(global_matrix.data(), sendcounts.data(), displs.data(), MPI_INT,
               local_matrix.data(), local_matrix.size(), MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(sendcounts.data(), sendcounts.size(), MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(rule.data(), rule.size(), MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(recv_displs.data(), recv_displs.size(), MPI_INT, 0, MPI_COMM_WORLD);
  local_matrix.resize(sendcounts[rank]);
  int j = 0;
  for (int i = 0; i < rule[rank]; i++) {
    int sum = 0;
    for (int j = 0; j < columns; j++)
      sum += local_matrix[j + i * columns];
    tmp.push_back(sum);
  }
  MPI_Gatherv(tmp.data(), tmp.size(), MPI_INT, result.data(), rule.data(),
              recv_displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
  return result;
}
