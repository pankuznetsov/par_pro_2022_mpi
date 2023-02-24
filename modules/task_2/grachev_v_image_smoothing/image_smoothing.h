// Copyright 2022 Grachev Valentin
#ifndef MODULES_TASK_2_GRACHEV_V_IMAGE_SMOOTHING_IMAGE_SMOOTHING_H_
#define MODULES_TASK_2_GRACHEV_V_IMAGE_SMOOTHING_IMAGE_SMOOTHING_H_

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

struct Direction {
    int x, y;
    Direction(int _x, int _y) {
        x = _x;
        y = _y;
    }
};

class Image {
 private:
    vector<unsigned char> pixels;
    int width;
    int height;

 public:
    Image(int _width, int _height);

    int GetWidth() const;
    int GetHeight() const;

    void FillRandom();
    unsigned char GetPixel(int x, int y) const;
    void SetPixel(int x, int y, unsigned char value);
    void Show();

    bool operator==(const Image &other) const;
};

vector<Direction> GetDirections();

Image GetCustomImage1();

Image SmoothingSequential(const Image &source, double *time);

Image SmoothingParallel(const Image &source, double *time);

#endif  // MODULES_TASK_2_GRACHEV_V_IMAGE_SMOOTHING_IMAGE_SMOOTHING_H_
