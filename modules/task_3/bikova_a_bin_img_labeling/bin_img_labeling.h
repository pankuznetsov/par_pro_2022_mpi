#ifndef BYKOVA_TASK_3_BIN_LABELEING
#define BYKOVA_TASK_3_BIN_LABELEING


#include <iostream>
#include "./mpi.h"

struct image {
  int m, n, count;
  int** data;

  image(int _m, int _n) : m(_m), n(_n), count(1) {
    data = new int*[m];
    for (int i = 0; i < m; i++) {
      data[i] = new int[n];
      for (int j = 0; j < n; j++) {
        data[i][j] = 0;
      }
    }
  }

  friend std::ostream& operator<< (std::ostream& os, const image& img) {
    for (int i = 0; i < img.m; i++) {
      for (int j = 0; j < img.n; j++) {
        os << img.data[i][j] << "\t";
      }
      os << std::endl;
    }
    return os;
  }
};

void labeling(image* img);

#endif 
