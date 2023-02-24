// Copyright 2022 me
#include "../../../modules/task_2/krolevets_n_gauss_by_columns/gauss_by_columns.h"

void Gaussian_Solver::generate_matrix() {
  std::mt19937 gen(42);
  std::uniform_real_distribution<> float_dist(-1000.f, 1000.f);
  orig_matrix.resize(size_x * size_y);
  matrix.resize(size_x * size_y);
  for (uint64_t i = 0; i < size_x * size_y; ++i) {
    orig_matrix[i] = matrix[i] = float_dist(gen);
  }
}

void Gaussian_Solver::permutate(uint32_t cur_column) {
  // here
  auto iter =
      std::max_element(matrix.begin() + cur_column * size_y + cur_column,
                       matrix.begin() + (cur_column + 1) * size_y);
  uint32_t max_index = (iter - matrix.begin()) % size_y;
  for (uint32_t i = 0; i < size_x; ++i) {
    std::swap(get(i, max_index), get(i, cur_column));
  }
}

std::vector<float> Gaussian_Solver::solve_sequential() {
  matrix = orig_matrix;
  std::vector<float> ans(size_y);
  for (uint32_t i = 0; i < size_y; ++i) {
    std::vector<float> mults(size_y, 0);
    permutate(i);
    for (uint32_t j = 0; j < size_y; ++j) {
      if (get(i, j) != 0 && get(i, i) != 0 && i != j) {
        mults[j] = get(i, j) / get(i, i);
      }
    }

    for (uint32_t j = i; j < size_x; ++j) {
      for (uint32_t k = 0; k < size_y; ++k) {
        if (mults[k] != 0) {
          get(j, k) -= get(j, i) * mults[k];
        }
      }
    }
  }
  for (uint32_t i = 0; i < size_y; ++i) {
    if (abs(get(i, i) - 0) < 0.0001f) {
      ans[i] = 0;
    } else {
      ans[i] = get(size_x - 1, i) / get(i, i);
    }
  }
  return ans;
}

std::vector<float> Gaussian_Solver::solve_parallel() {
  matrix = orig_matrix;

  int32_t rank, pcount;
  MPI_Status status;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &pcount);

  std::vector<float> mults(size_y);

  uint32_t columns_per_process = size_x / pcount;

  if ((uint32_t)rank < size_x && rank != 0) {
    matrix.resize(size_y * columns_per_process);
  }

  for (uint32_t i = 0; i < size_y; ++i) {
    std::fill(mults.begin(), mults.end(), 0);
    uint32_t min_p_id = 1;
    uint32_t max_p_id = std::min((uint32_t)pcount, size_x);
    if (rank == 0) {
      permutate(i);
      for (uint32_t j = 0; j < size_y; ++j) {
        if (get(i, j) != 0 && get(i, i) != 0 && i != j) {
          mults[j] = get(i, j) / get(i, i);
        }
      }
      MPI_Bcast(mults.data(), size_y, MPI_FLOAT, 0, MPI_COMM_WORLD);
      for (uint32_t p_id = min_p_id; p_id < max_p_id; ++p_id) {
        MPI_Send(matrix.data() + size_y * p_id * columns_per_process,
                 columns_per_process * size_y, MPI_FLOAT, p_id, 0,
                 MPI_COMM_WORLD);
      }
      for (uint32_t j = std::max(0u, i); j < columns_per_process; ++j) {
        for (uint32_t k = 0; k < size_y; ++k) {
          if (mults[k] != 0) {
            get(j, k) -= get(j, i) * mults[k];
          }
        }
      }
      for (uint32_t p_id = min_p_id; p_id < max_p_id; ++p_id) {
        MPI_Recv(matrix.data() + size_y * p_id * columns_per_process,
                 columns_per_process * size_y, MPI_FLOAT, p_id, 0,
                 MPI_COMM_WORLD, &status);
      }
      for (uint32_t j = pcount * columns_per_process; j < size_x; ++j) {
        for (uint32_t k = 0; k < size_y; ++k) {
          if (mults[k] != 0) {
            get(j, k) -= get(j, i) * mults[k];
          }
        }
      }
    } else if ((uint32_t)rank < max_p_id) {
      MPI_Bcast(mults.data(), size_y, MPI_FLOAT, 0, MPI_COMM_WORLD);
      MPI_Recv(matrix.data(), size_y * columns_per_process, MPI_FLOAT, 0, 0,
               MPI_COMM_WORLD, &status);
      uint32_t j = 0;
      if ((rank + 1) * columns_per_process <= i) {
        j = columns_per_process;
      } else if (rank * columns_per_process <= i) {
        j = i % (rank * columns_per_process);
      } else {
        j = 0;
      }
      for (; j < columns_per_process; ++j) {
        for (uint32_t k = 0; k < size_y; ++k) {
          if (mults[k] != 0) {
            get(j, k) -= get(j, i) * mults[k];
          }
        }
      }
      MPI_Send(matrix.data(), size_y * columns_per_process, MPI_FLOAT, 0, 0,
               MPI_COMM_WORLD);
    }
  }

  std::vector<float> ans(size_y);
  if (rank == 0) {
    for (uint32_t i = 0; i < size_y; ++i) {
      if (abs(get(i, i) - 0) < 0.0001f) {
        ans[i] = 0;
      } else {
        ans[i] = get(size_x - 1, i) / get(i, i);
      }
    }
  }
  MPI_Bcast(ans.data(), size_y, MPI_FLOAT, 0, MPI_COMM_WORLD);
  return ans;
}

float Gaussian_Solver::check_answer(const std::vector<float>& ans) {
  float max_delta = 0;
  for (uint32_t i = 0; i < size_y; ++i) {
    float b_i = 0;
    for (uint32_t j = 0; j < size_x - 1; ++j) {
      b_i += ans[j] * get_orig(j, i);
    }
    max_delta = std::max(std::abs(b_i - get_orig(size_x - 1, i)), max_delta);
  }
  return max_delta;
}
