// Copyright 2022 me
#include "../../../modules/task_3/krolevets_n_shell_merge_sort/shell_merge_sort.h"

uint64_t next_power_of_2(uint64_t n) {
  n--;

  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  n |= n >> 32;

  return ++n;
}

std::vector<float> generate_array(uint64_t size) {
  std::vector<float> res(size);
  std::mt19937 gen(42);
  std::uniform_real_distribution<> float_dist(-1000000.f, 1000000.f);
  for (uint64_t i = 0; i < res.size(); ++i) {
    res[i] = float_dist(gen);
  }
  return res;
}

void shell_sort(float* src, uint64_t size) {
  for (uint64_t gap = size / 2; gap > 0; gap >>= 1) {
    for (uint64_t i = gap; i < size; ++i) {
      float temp = src[i];
      uint64_t j;
      for (j = i; j >= gap && src[j - gap] > temp; j -= gap) {
        src[j] = src[j - gap];
      }
      src[j] = temp;
    }
  }
}

void merge(float* src, uint64_t size) {
  float* res = new float[size];
  uint64_t ind = 0;
  uint64_t ind1 = 0;
  uint64_t ind2 = size / 2;
  while (ind1 < size / 2 && ind2 < size) {
    if (src[ind1] <= src[ind2]) {
      res[ind++] = src[ind1++];
    } else {
      res[ind++] = src[ind2++];
    }
  }
  while (ind1 < size / 2) {
    res[ind++] = src[ind1++];
  }
  while (ind2 < size) {
    res[ind++] = src[ind2++];
  }
  std::memcpy(src, res, sizeof(float) * size);
}

bool check(const std::vector<float>& to_check) {
  for (uint64_t i = 1; i < to_check.size(); ++i) {
    if (to_check[i - 1] > to_check[i]) {
      return false;
    }
  }
  return true;
}

std::vector<float> merge_sort_par(const std::vector<float>& src) {
  int32_t rank, pcount;
  MPI_Status status;

  std::vector<float> local_vec = src;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &pcount);

  uint64_t arrays_to_sort = next_power_of_2(pcount);
  uint64_t array_size =
      (local_vec.size() + arrays_to_sort - 1) / arrays_to_sort;

  if (rank == 0) {
    MPI_Bcast(&array_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    uint64_t original_size = local_vec.size();
    local_vec.resize(arrays_to_sort * array_size);
    if (arrays_to_sort * array_size > original_size) {
      std::fill(local_vec.begin() + original_size, local_vec.end(),
                std::numeric_limits<float>::max());
    }
    for (uint64_t i = 1; i < arrays_to_sort; ++i) {
      int32_t p_rank = i % pcount;
      if (p_rank == 0) {
        continue;
      }
      MPI_Send(local_vec.data() + array_size * i, array_size, MPI_FLOAT, p_rank,
               0, MPI_COMM_WORLD);
    }
    for (uint64_t i = 0; i * pcount < arrays_to_sort; ++i) {
      shell_sort(local_vec.data() + array_size * i * pcount, array_size);
    }
    for (uint64_t i = 1; i < arrays_to_sort; ++i) {
      int32_t p_rank = i % pcount;
      if (p_rank == 0) {
        continue;
      }
      MPI_Recv(local_vec.data() + array_size * i, array_size, MPI_FLOAT, p_rank,
               0, MPI_COMM_WORLD, &status);
    }

    while (arrays_to_sort != 1) {
      arrays_to_sort >>= 1;
      array_size <<= 1;
      for (uint64_t i = 1; i < arrays_to_sort; ++i) {
        uint64_t p_rank = i % pcount;
        if (p_rank == 0) {
          continue;
        }
        MPI_Send(local_vec.data() + array_size * i, array_size, MPI_FLOAT,
                 p_rank, 0, MPI_COMM_WORLD);
      }
      for (uint64_t i = 0; i * pcount < arrays_to_sort; ++i) {
        merge(local_vec.data() + array_size * i * pcount, array_size);
      }
      for (uint64_t i = 1; i < arrays_to_sort; ++i) {
        uint64_t p_rank = i % pcount;
        if (p_rank == 0) {
          continue;
        }
        MPI_Recv(local_vec.data() + array_size * i, array_size, MPI_FLOAT,
                 p_rank, 0, MPI_COMM_WORLD, &status);
      }
    }
    local_vec.resize(original_size);
    return local_vec;
  } else {
    MPI_Bcast(&array_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    uint64_t iterations =
        arrays_to_sort / pcount +
        (arrays_to_sort % (uint64_t)pcount > (uint64_t)rank ? 1 : 0);
    auto calc_merge_iterations = [](uint64_t num_of_arrays,
                                    uint64_t array_size) {
      while (num_of_arrays != 1) {
        array_size <<= 1;
        num_of_arrays >>= 1;
      }
      return array_size;
    };
    uint64_t new_local_vec_size =
        std::max(iterations * array_size,
                 calc_merge_iterations(arrays_to_sort, array_size));
    local_vec.resize(new_local_vec_size);
    for (uint64_t i = 0; i < iterations; ++i) {
      MPI_Recv(local_vec.data() + i * array_size, array_size, MPI_FLOAT, 0, 0,
               MPI_COMM_WORLD, &status);
      shell_sort(local_vec.data() + array_size * i, array_size);
    }
    for (uint64_t i = 0; i < iterations; ++i) {
      MPI_Send(local_vec.data() + i * array_size, array_size, MPI_FLOAT, 0, 0,
               MPI_COMM_WORLD);
    }

    while (arrays_to_sort != 1) {
      arrays_to_sort >>= 1;
      array_size <<= 1;
      iterations = arrays_to_sort / pcount +
                   (arrays_to_sort % (uint64_t)pcount > (uint64_t)rank ? 1 : 0);
      for (uint64_t i = 0; i < iterations; ++i) {
        MPI_Recv(local_vec.data() + i * array_size, array_size, MPI_FLOAT, 0, 0,
                 MPI_COMM_WORLD, &status);
      }
      for (uint64_t i = 0; i * pcount < arrays_to_sort; ++i) {
        merge(local_vec.data(), array_size);
      }
      for (uint64_t i = 0; i < iterations; ++i) {
        MPI_Send(local_vec.data() + i * array_size, array_size, MPI_FLOAT, 0, 0,
                 MPI_COMM_WORLD);
      }
    }
    return std::vector<float>();
  }
}
