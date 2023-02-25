// Copyright 2023 Kuznetsov Sergey
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_1/kuznetsov_s_symbol_freq/symbol_freq.h"

using namespace std;

std::string randomString(int length) {
  std::string s(length, 0);
  for (int i = 0; i < length; i++) s[i] = 'a' + rand() % 26;
  return s;
}

int symbolFrequencySequential(std::string& str, char symbol) {
  int counter = 0;
  for (int i = 0; i < str.length(); i++)
    if (str[i] == symbol) counter++;
  return counter;
}

/* int symbolFrequencyParallel(std::string& str, char symbol) {
  int np, pid; // np - number of the processes, pid - it's identifier
  MPI_Status status;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  // cout << "pid: " << pid << endl << "np: " << np << endl;
  // master process
  int totalSum = 0;
  int epp = str.length() / np;
  // cout << "len: " << str.length() << endl;
  // cout << "epp: " << epp << endl;
  if (pid != 0)  {
    cout << "pid: " << pid << ", s: " << epp * (pid - 1) << ", e: " << epp * pid << endl;
    int p_sum = symbolFrequencySequential(str, epp * (pid - 1), epp * pid, symbol);
    cout << "p_sum: " << p_sum << endl;
    MPI_Send(&p_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  } else {
    int tmp = 0;
    for (int i = 1; i < np - 1; i++) {
      if (i == np) break;
      // cout << "RECV" << endl;
      MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
      totalSum += tmp;
      // cout << "tmp: " << tmp << endl;
    }
    cout << "central" << ", s: " << (np - 1) * epp << ", e: " << str.length() << endl;
    cout << "totalSum: " << totalSum << endl;
    totalSum += symbolFrequencySequential(str, (np - 1) * epp, str.length(), symbol);
    cout << "totalSumA: " << totalSum << endl;
  }
  cout << "-----------" << endl;
  // MPI_Barrier(MPI_COMM_WORLD);
  return totalSum;
} */

int symbolFrequencyParallel(std::string& str, char symbol) {
  int size, rank;

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int delta = str.length() / size;
  int remain = str.length() - delta * size;

  if (rank == 0) {
    for (int process_num = 1; process_num < size; process_num++)
      MPI_Send(&str[remain + process_num * delta], delta, MPI_CHAR,
               process_num, 0, MPI_COMM_WORLD);
  }

  std::string part_string;
  part_string.resize(rank == 0 ? delta + remain : delta);

  if (rank == 0) {
    part_string = { str.begin(), str.begin() + delta + remain };
  } else {
    MPI_Status status;
    MPI_Recv(&part_string[0], delta, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
  }

  int sum = 0;
  int part_sum = symbolFrequencySequential(part_string, symbol);

  MPI_Reduce(&part_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  return sum;
}