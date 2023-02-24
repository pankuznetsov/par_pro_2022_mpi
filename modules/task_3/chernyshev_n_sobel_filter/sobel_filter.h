// Copyright 2022 Chernyshev Nikita
#ifndef MODULES_TASK_3_CHERNYSHEV_N_SOBEL_FILTER_SOBEL_FILTER_H_
#define MODULES_TASK_3_CHERNYSHEV_N_SOBEL_FILTER_SOBEL_FILTER_H_

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>
using std::cout;
using std::vector;

class ByteImage {
 private:
    vector<unsigned char> pixels;
    int width;
    int height;

 public:
    ByteImage(int _width, int _height);
    ByteImage(const ByteImage &image);

    int Width() const;
    int Height() const;

    void Randomize();
    unsigned char GetPixel(int x, int y) const;
    void SetPixel(int x, int y, unsigned char value);
    void Print();
    unsigned char *GetPixelsRows(int from_y_inclusive,
                                 int to_y_exclusive) const;

    bool operator==(const ByteImage &other) const;
};

unsigned char SobelPixelHandle(vector<unsigned char> around_pixels);

ByteImage GetCustomImage();

ByteImage SobelSequential(const ByteImage &origin);

ByteImage SobelParallel(const ByteImage &origin);

#endif  // MODULES_TASK_3_CHERNYSHEV_N_SOBEL_FILTER_SOBEL_FILTER_H_
