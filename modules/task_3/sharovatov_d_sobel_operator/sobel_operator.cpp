// Copyright 2022 Sharovatov Daniil
#include "../../../modules/task_3/sharovatov_d_sobel_operator/sobel_operator.h"

std::vector<std::vector<int>> getRandomImage(int width, int height) {
  std::vector<std::vector<int>> randomImage(height, std::vector<int>(width));

  std::random_device dev;
  std::mt19937 gen(dev());

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      randomImage[i][j] = gen() % 256;
    }
  }

  return randomImage;
}

std::vector<int> matrixToVector(std::vector<std::vector<int>> imageMatrix) {
  int height = imageMatrix.size();
  int width = imageMatrix[0].size();

  std::vector<int> imageVector(width * height);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      imageVector[i * width + j] = imageMatrix[i][j];
    }
  }

  return imageVector;
}

std::vector<std::vector<int>> vectorToMatrix(std::vector<int> imageVector, int height) {
  int width = imageVector.size() / height;
  std::vector<std::vector<int>> imageMatrix(height, std::vector<int>(width));

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      imageMatrix[i][j] = imageVector[i * width + j];
    }
  }

  return imageMatrix;
}

int clamp(int value, int min, int max) {
  return std::min(std::max(value, min), max);
}

int calculateNewPixelColor(std::vector<std::vector<int>> image, int x, int y) {
  std::vector<std::vector<int>> kernel = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

  int height = image.size();
  int width = image[0].size();

  int radiusX = 1;
  int radiusY = 1;

  int resultX = 0;
  int resultY = 0;

  for (int l = -radiusY; l <= radiusY; l++) {
    for (int k = -radiusX; k <= radiusX; k++) {
      int idX = clamp(x + k, 0, width - 1);
      int idY = clamp(y + l, 0, height - 1);
      int neighborColor = image[idY][idX];

      resultX += neighborColor * kernel[l + radiusY][k + radiusX];
      resultY += neighborColor * kernel[k + radiusX][l + radiusY];
    }
  }

  int resColor = clamp((sqrt(pow(resultX, 2) + pow(resultY, 2))), 0, 255);
  return resColor;
}

std::vector<std::vector<int>> sobelOperatorSequential(
    std::vector<std::vector<int>> image) {
  int height = image.size();
  int width = image[0].size();

  std::vector<std::vector<int>> resultImage(height, std::vector<int>(width));

  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++) {
      resultImage[i][j] = calculateNewPixelColor(image, i, j);
    }

  return resultImage;
}

std::vector<std::vector<int>> sobelOperatorParallel(
    std::vector<std::vector<int>> image) {
  int ProcNum, ProcRank;
  MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

  int height = image.size();
  int width = image[0].size();

  int rowsNum = image.size() / ProcNum;
  int remain = image.size() % ProcNum;

  std::vector<std::vector<int>> newImageMatrix(height, std::vector<int>(width));
  std::vector<int> newImageVector = std::vector<int>(height * width);

  std::vector<std::vector<int>> newLocalImage(rowsNum, std::vector<int>(width));
  if (ProcRank == ProcNum - 1) {
    newLocalImage.resize(rowsNum + remain, std::vector<int>(width));
  }

  for (int i = 0; i < rowsNum; i++) {
    int rowIndx = rowsNum * ProcRank + i;
    std::vector<int> row = image[rowIndx];

    for (int j = 0; j < row.size(); j++) {
      newLocalImage[i][j] = calculateNewPixelColor(image, rowIndx, j);
    }
  }

  if (ProcRank == ProcNum - 1) {
    for (int i = 0; i < remain; i++) {
      int rowIndx = rowsNum * (ProcRank + 1) + i;
      std::vector<int> row = image[rowIndx];

      for (int j = 0; j < row.size(); j++) {
        newLocalImage[rowsNum + i][j] =
            calculateNewPixelColor(image, rowIndx, j);
      }
    }
  }

  MPI_Gather(matrixToVector(newLocalImage).data(), rowsNum * width, MPI_INT,
             newImageVector.data(), rowsNum * width, MPI_INT, 0,
             MPI_COMM_WORLD);

  if (ProcNum > 1) {
    if (ProcRank == ProcNum - 1) {
      MPI_Send(matrixToVector(newLocalImage).data() + rowsNum * width,
               remain * width, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    if (ProcRank == 0) {
      MPI_Status status;
      MPI_Recv(newImageVector.data() + rowsNum * ProcNum * width,
               remain * width, MPI_INT, ProcNum - 1, 0, MPI_COMM_WORLD,
               &status);
    }
  }

  MPI_Bcast(newImageVector.data(), height * width, MPI_INT, 0, MPI_COMM_WORLD);

  newImageMatrix = vectorToMatrix(newImageVector, height);

  return newImageMatrix;
}
