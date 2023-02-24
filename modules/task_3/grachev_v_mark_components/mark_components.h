// Copyright 2022 Grachev Valentin
#ifndef MODULES_TASK_3_GRACHEV_V_MARK_COMPONENTS_MARK_COMPONENTS_H_
#define MODULES_TASK_3_GRACHEV_V_MARK_COMPONENTS_MARK_COMPONENTS_H_

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>
#include <set>
using std::cout;
using std::set;
using std::vector;

struct MarkPair {
    int lower_mark, upper_mark;

    bool operator<(const MarkPair &other) const {
        if (upper_mark == other.upper_mark)
            return lower_mark < other.lower_mark;
        else
            return upper_mark < other.upper_mark;
    }
};

struct Coord {
    int x, y;
};

class Image {
 private:
    vector<int> pixels;
    int width;
    int height;

 public:
    Image(int _width, int _height);
    Image(const Image &other);

    int GetWidth() const;
    int GetHeight() const;

    void FillRandomBinary();
    int GetPixel(int x, int y) const;
    void SetPixel(int x, int y, int value);
    bool PixelExists(int x, int y) const;
    int *ToDataMPI() const;
    void Show() const;
    Image GetImageStrings(int fromY_inclusive, int toY_exclusive) const;

    bool operator==(const Image &other) const;
};

Image GetPreset1();

void MoveCoordVector(vector<Coord> *to, vector<Coord> *from);

Image PreMarking(const Image &source, int *last_mark, int start_mark);

Image PostMarking(const Image &premarking_image, int last_mark);

Image MarkingSequential(const Image &source, double *time);

Image MarkingParallel(const Image &source, double *time);

Image GetImageFromDataMPI(int *data_mpi);

bool CheckMarking(const Image &marked_image);

#endif  // MODULES_TASK_3_GRACHEV_V_MARK_COMPONENTS_MARK_COMPONENTS_H_
