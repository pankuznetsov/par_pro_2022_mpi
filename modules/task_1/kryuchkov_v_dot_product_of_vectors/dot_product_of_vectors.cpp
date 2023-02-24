// Copyright 2022 Kryuchkov Vladimir
#include <mpi.h>
#include <numeric>
#include <random>
#include "../../../modules/task_1/kryuchkov_v_dot_product_of_vectors/dot_product_of_vectors.h"

int get_sequential_product(const std::vector<int>& a,
                           const std::vector<int>& b) {
  int res = 0;
  if (a.size() != b.size()) {
    return -1;
  }
  for (std::vector<int>::size_type i = 0; i < a.size(); i++) {
    res += a[i] * b[i];
  }
  return res;
}

int get_sequential_product_std(const std::vector<int>& a,
                               const std::vector<int>& b) {
  int res = std::inner_product(a.begin(), a.end(), b.begin(), 0);
  return res;
}

std::vector<int> get_random_vector(int size) {
  std::vector<int> vector(size);
  std::random_device rand_device;
  std::mt19937 rand_r(rand_device());
  for (int i = 0; i < size; i++) {
    vector[i] = rand_r() % 100;
  }
  return vector;
}

int get_parallel_product(std::vector<int> a, std::vector<int> b) {
  int size = a.size();
  int res = 0, local_res = 0;
  int proc_count = 0, proc_rank = 0;
  if (a.size() != b.size()) {
    return -1;
  }
  MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
  int local_size = size / proc_count;
  std::vector<int> local_a(local_size);
  std::vector<int> local_b(local_size);
  if (proc_rank == 0) {
    if (size % proc_count != 0) {
      for (int i = size - size % proc_count; i < size; i++) {
        local_a.push_back(a[i]);
        local_b.push_back(b[i]);
      }
    }
  }
  MPI_Scatter(a.data(), local_size, MPI_INT, local_a.data(), local_size,
              MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(b.data(), local_size, MPI_INT, local_b.data(), local_size,
              MPI_INT, 0, MPI_COMM_WORLD);
  local_res = get_sequential_product_std(local_a, local_b);
  MPI_Reduce(&local_res, &res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  return res;
}
