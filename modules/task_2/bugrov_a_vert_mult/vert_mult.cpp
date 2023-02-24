// Copyright 2022 Bugrov Andrey

#include "../../../modules/task_2/bugrov_a_vert_mult/vert_mult.h"

#include <mpi.h>  // for mpi

#include <ctime>   // for time function
#include <random>  // for mt19937

void RandCreator(int* matrix_or_vector, int size) {
  std::mt19937 engine(time(0));
  for (int i = 0; i < size; i++) {
    matrix_or_vector[i] = engine() % 97;
  }
}

// matrix is loaded by columns
void SeqMult(int* matrix, int* vector, int* result_vector, int n, int m) {
  for (int j = 0; j < n; j++) {
    for (int i = 0; i < m; i++) {
      result_vector[j] += matrix[i * n + j] * vector[i];
    }
  }
}

void ParMult(int* matrix, int* vector, int* result_vector, int n, int m) {
  int rank;
  int process_num;
  MPI_Comm_size(MPI_COMM_WORLD, &process_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int part = m / process_num;
  int remainder = m % process_num;
  int* sendcounts = new int[process_num];  // parts of matrix
                                           // (measured in number of elements)
  int* send_vector_counts = new int[process_num];
  int* recvbuf;     // part of matrix for current processor
  int* recvvector;  // part of vector for current processor
  int* displs = new int[process_num];
  int* vec_displs = new int[process_num];

  int i;

  // displs[0] = vec_displs[0] = 0;
  // if (remainder > 0) {
  //  sendcounts[i] = (part + 1) * n;
  //  send_vector_counts[i] = part + 1;
  //  remainder--;
  //} else {
  //  sendcounts[i] = part * n;
  //  send_vector_counts[i] = part;
  //}
  // for (; i < process_num; i++) {
  //  if (remainder > 0) {
  //    sendcounts[i] = (part + 1) * n;
  //    send_vector_counts[i] = part + 1;
  //    remainder--;
  //  } else {
  //    sendcounts[i] = part * n;
  //    send_vector_counts[i] = part;
  //  }
  //  displs[i] = displs[i - 1] + sendcounts[i];
  //  vec_displs[i] = vec_displs[i - 1] + send_vector_counts[i];
  //}

  for (i = 0; i < process_num - 1; i++) {
    sendcounts[i] = part * n;
    send_vector_counts[i] = part;
    displs[i] = i * part * n;
    vec_displs[i] = i * part;
  }
  sendcounts[process_num - 1] = part * n + remainder * n;
  send_vector_counts[process_num - 1] = part + remainder;
  displs[process_num - 1] = part * n * (process_num - 1);
  vec_displs[process_num - 1] = part * (process_num - 1);

  recvbuf = new int[sendcounts[rank]];
  recvvector = new int[send_vector_counts[rank]];
  int* cur_result_vector = new int[n];
  for (i = 0; i < n; i++) {
    cur_result_vector[i] = 0;
  }

  /*int MPI_Scatterv(const void* sendbuf, const int sendcounts[],
                     const int displs[], MPI_Datatype sendtype, void* recvbuf,
                     int recvcount, MPI_Datatype recvtype, int root,
                     MPI_Comm comm);*/
  /*int MPI_Reduce(const void* sendbuf, void* recvbuf, int count,
                MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)*/
  MPI_Scatterv(matrix, sendcounts, displs, MPI_INT, recvbuf, sendcounts[rank],
               MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatterv(vector, send_vector_counts, vec_displs, MPI_INT, recvvector,
               send_vector_counts[rank], MPI_INT, 0, MPI_COMM_WORLD);
  SeqMult(recvbuf, recvvector, cur_result_vector, n, send_vector_counts[rank]);
  MPI_Reduce(cur_result_vector, result_vector, n, MPI_INT, MPI_SUM, 0,
             MPI_COMM_WORLD);
}
