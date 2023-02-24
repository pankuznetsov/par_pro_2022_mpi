// Copyright 2022 Sukharev Artem

#include "./count_letters_in_string.h"

#include <mpi.h>

#include <algorithm>
#include <random>
#include <string>

std::string getRandomString(int size) {
  std::random_device dev;
  std::mt19937 gen(dev());
  std::string randomString;
  randomString.resize(size);
  for (int i = 0; i < size; i++) {
    randomString[i] = static_cast<char>(static_cast<int>('a') + gen() % 26);
  }
  return randomString;
}

char getRandomLetter() {
  std::random_device dev;
  std::mt19937 gen(dev());
  return static_cast<char>(static_cast<int>('a') + gen() % 26);
}

int countLettersSequential(const std::string& localString, const char letter) {
  const int size = localString.size();
  int amount = 0;
  for (int i = 0; i < size; i++) {
    if (localString[i] == letter) amount++;
  }
  return amount;
}

int countLettersParallel(const std::string& globalString, char letter) {
  int comm_size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (comm_size == 1) {
    return countLettersSequential(globalString, letter);
  }

  int stringSize = 0;
  if (rank == 0) {
    stringSize = globalString.size();
  }
  MPI_Bcast(&stringSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&letter, 1, MPI_CHAR, 0, MPI_COMM_WORLD);

  int localSize = stringSize / comm_size;
  int rest = stringSize - localSize * (comm_size - 1);
  std::string localString;
  if (rank == 0) {
    localString = std::string(globalString.end() - rest, globalString.end());
    for (int i = 0; i < comm_size - 1; i++) {
      MPI_Send(globalString.data() + localSize * i, localSize, MPI_CHAR, i + 1,
               0, MPI_COMM_WORLD);
    }
  } else {
    char* tmp = new char[localSize + 1];
    MPI_Status status;
    MPI_Recv(tmp, localSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
    tmp[localSize] = '\0';
    localString = std::string(tmp);
  }

  int localAmount = countLettersSequential(localString, letter);
  int globalAmount = 0;
  MPI_Reduce(&localAmount, &globalAmount, 1, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);
  return globalAmount;
}
