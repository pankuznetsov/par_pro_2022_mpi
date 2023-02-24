// Copyright 2023 Kuleva Anna
#include <mpi.h>
#include <iostream>
#include <random>
#include <iterator>
#include <algorithm>
#include "../../../modules/task_3/kuleva_a_gauss_filter/gauss_filter.h"

CoreMatrix coreMatrix = {
    {1.0 / 16.0, 1.0 / 8.0, 1.0 / 16.0},
    {1.0 / 8.0, 1.0 / 4.0, 1.0 / 8.0},
    {1.0 / 16.0, 1.0 / 8.0, 1.0 / 16.0}
};

Matrix generateImage(int sizeX, int sizeY) {
    std::mt19937 generator(1);
    std::uniform_int_distribution<> randPixel(0, 255);
    Matrix result;
    result.reserve(sizeX * sizeY);
    for (int i = 0; i < sizeX * sizeY; i++) {
        result.push_back(randPixel(generator));
    }
    return result;
}

int translateCoordinates(int x, int y, int sizeX) {
    return y * sizeX + x;
}

unsigned char getGauseFilterForPixel(const Matrix& matrix, int x, int y, int sizeX) {
    const int halfCoreSize = coreMatrix.size() / 2;
    double sum = 0.0;
    for (int i = -halfCoreSize; i <= halfCoreSize; i++) {
        for (int j = -halfCoreSize; j <= halfCoreSize; j++) {
            const int currentX = x + i;
            const int currentY = y + j;
            const int linCoords = translateCoordinates(currentX, currentY, sizeX);
            const double coreMatrixElement = coreMatrix[i + halfCoreSize][j + halfCoreSize];
            sum += coreMatrixElement * matrix[linCoords];
        }
    }
    return static_cast<unsigned char>(sum);
}

Matrix applyGauseFilterPar(const Matrix& image, int sizeX, int sizeY) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int rowsCount = sizeY - 2;
    const int rowPerOneProc = rowsCount / size + ((rowsCount % size) != 0);
    const int pixelPerOneProcCount = rowPerOneProc * sizeX;

    Matrix tempImage = image;
    tempImage.resize(sizeX * sizeY);
    MPI_Bcast(tempImage.data(), tempImage.size(), MPI_CHAR, 0, MPI_COMM_WORLD);

    const int currentProcBeginRow = rowPerOneProc * rank + 1;
    const int currentProcEndRow = std::min(rowPerOneProc * (rank + 1) + 1, sizeY - 1);

    Matrix procResult;
    procResult.reserve(pixelPerOneProcCount);
    for (int i = currentProcBeginRow; i < currentProcEndRow; i++) {
        for (int j = 0; j < sizeX; j++) {
            const int pos = translateCoordinates(j, i, sizeX);
            if (j == 0 || j == sizeX - 1) {
                procResult.push_back(tempImage[pos]);
                continue;
            }
            const unsigned char newPixelValue = getGauseFilterForPixel(tempImage, j, i, sizeX);
            procResult.push_back(newPixelValue);
        }
    }

    Matrix result;
    if (rank == 0) {
        std::copy(image.begin(), image.begin() + sizeX, std::back_inserter(result));
        result.resize(pixelPerOneProcCount * size + sizeX);
    }

    MPI_Gather(procResult.data(), pixelPerOneProcCount, MPI_CHAR,
        result.data() + sizeX, pixelPerOneProcCount, MPI_CHAR, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        result.resize(sizeX * (sizeY - 1));
        std::copy(image.end() - sizeX, image.end(), std::back_inserter(result));
    }
    return result;
}
