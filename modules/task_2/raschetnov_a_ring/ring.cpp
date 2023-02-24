// Copyright 2023 Raschetnov Alexei
#include <mpi.h>
#include <random>
#include "../../../modules/task_2/raschetnov_a_ring/ring.h"

int getResult(int information, int size, int delta) {
  for (int i = 0; i < size - 1; i++)
    information += delta;
  return information;
}

std::vector<int> constructPath(int root, int destination, int size) {
  std::vector<int> result;
  int i = root;
  while (i != destination) {
    result.push_back(i);
    i = (i + 1) % size;
  }
  result.push_back(i);
  return result;
}

int SendRingParallel(int information, int count, MPI_Datatype datatype,
                     int root, int destination, int delta, int tag, MPI_Comm comm) {
  if (root == destination)
    return information;
  int world_size, world_rank;
  MPI_Comm new_comm;
  MPI_Group old_group, new_group;

  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  std::vector<int> ranks = constructPath(root, destination, world_size);
  MPI_Comm_group(MPI_COMM_WORLD, &old_group);
  MPI_Group_incl(old_group, ranks.size(), ranks.data(), &new_group);

  MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);

  int local_rank, local_size;

  if (MPI_COMM_NULL != new_comm) {
    MPI_Status status;
    MPI_Comm_rank(new_comm, &local_rank);
    MPI_Comm_size(new_comm, &local_size);
    if (local_rank == 0) {
      MPI_Send(&information, 1, MPI_INT, (local_rank + 1) % local_size, 0, new_comm);
      int source = local_size - 1;
      MPI_Recv(&information, 1, MPI_INT, source, 0, new_comm, &status);
      return information;
    } else {
    int source = local_rank - 1;
    if (local_rank == 0)
      source = local_size - 1;
    MPI_Recv(&information, 1, MPI_INT, source, 0, new_comm, &status);
    information += delta;
    MPI_Send(&information, 1, MPI_INT, (local_rank + 1) % local_size, 0, new_comm);
    }
  }
  return information;
}

int getRandomNumber(int leftborder, int rightborder) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(leftborder, rightborder);

  return distr(gen);
}
