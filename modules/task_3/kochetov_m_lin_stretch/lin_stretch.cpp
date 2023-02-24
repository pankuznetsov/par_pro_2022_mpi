// Copyright 2022 Kochetov Maksim
#include "../../../modules/task_3/kochetov_m_lin_stretch/lin_stretch.h"
#include <mpi.h>
#include <algorithm>
#include <iostream>
#include <vector>


ColorChannel stretchTon(ColorChannel ton, ColorChannel min, ColorChannel max) {
    const int diff = max - min;
    const ColorChannel stretched = (ton - min) * 255 / diff;
    return stretched;
}

void stretchBitmap(Bitmap* sourcePtr, ColorChannel minTon, ColorChannel maxTon) {
    Bitmap& source = *sourcePtr;
    for (int i = 0; i < source.size(); i++) {
        const ColorChannel sourceTon = source[i];
        source[i] = stretchTon(sourceTon, minTon, maxTon);
    }
}

Bitmap genBitmap(int size, int minTon, int maxTon) {
    Bitmap result(size);
    ColorChannel currentTon = minTon;
    for (ColorChannel& ton : result) {
        ton = currentTon++;
        currentTon = (currentTon - minTon) % (maxTon - minTon) + minTon;
    }
    return result;
}

Bitmap getLinearStretchingSequential(const Bitmap& source) {
    const auto minMax = std::minmax_element(source.begin(), source.end());
    const ColorChannel minTon = *minMax.first;
    const ColorChannel maxTon = *minMax.second;

    Bitmap result = source;
    stretchBitmap(&result, minTon, maxTon);

    return result;
}

Bitmap getLinearStretchingParralel(const Bitmap& source) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    ColorChannel minTon = 0, maxTon = 0;
    if (rank == 0) {
        const auto minMax = std::minmax_element(source.begin(), source.end());
        minTon = *minMax.first;
        maxTon = *minMax.second;
    }

    MPI_Bcast(&minTon, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(&maxTon, 1, MPI_CHAR, 0, MPI_COMM_WORLD);

    int pixelCountForOneProc = source.size() / size;
    int remains = source.size() % size;
    pixelCountForOneProc += (remains != 0);

    MPI_Bcast(&pixelCountForOneProc, 1, MPI_INT, 0, MPI_COMM_WORLD);

    const int extendedBitmapSize = pixelCountForOneProc * size;

    Bitmap result = source;
    if (rank == 0) {
        result.resize(extendedBitmapSize);
    }

    Bitmap localBitmap(pixelCountForOneProc);
    MPI_Scatter(result.data(), pixelCountForOneProc, MPI_CHAR,
        localBitmap.data(), pixelCountForOneProc, MPI_CHAR, 0, MPI_COMM_WORLD);

    stretchBitmap(&localBitmap, minTon, maxTon);

    MPI_Gather(localBitmap.data(), pixelCountForOneProc, MPI_CHAR,
        result.data(), pixelCountForOneProc, MPI_CHAR, 0, MPI_COMM_WORLD);

    result.resize(source.size());

    return result;
}
