// Copyright 2023 Melnik Denis
#include "../../../modules/task_2/melnik_d_lent_horiz_scheme/lent_horiz_scheme.h"

Matrix::Matrix(int h, int w) : matrix(w * h) {
    width = w;
    height = h;
}

Matrix Matrix::ToMatrix(int *arr, int h, int w) {
    Matrix res(h, w);
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++)
            res.SetValue(i, j, arr[i * w + j]);

    return res;
}

Matrix Matrix::GetRandom(int h, int w) {
    std::random_device dev;
    std::mt19937 gen(dev());
    Matrix result(h, w);

    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            int random = -maxInt;
            random += gen() % (2 * maxInt);
            result.SetValue(i, j, random);
        }

    return result;
}

vector<int> Sequence(const Matrix &originMatrix,
                     const vector<int> &originVector) {
    vector<int> result(originMatrix.H());

    for (int i = 0; i < originMatrix.H(); i++) {
        result[i] = 0;
        for (int j = 0; j < originMatrix.W(); j++)
            result[i] += originMatrix.GetValue(i, j) * originVector[j];
    }
    return result;
}

vector<int> Parallel(const Matrix &originMatrix,
                     const vector<int> &originVector) {
    int proccesors;
    MPI_Comm_size(MPI_COMM_WORLD, &proccesors);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        int procLines = originMatrix.H() / proccesors;
        int mainProcLines = procLines + originMatrix.H() % proccesors;

        for (int procNumber = 1; procNumber < proccesors; procNumber++) {
            int offset = mainProcLines + procLines * (procNumber - 1);
            int *matrArr = new int[procLines * originMatrix.W()];
            int *vectArr = new int[originVector.size()];

            int arrIndex = 0;
            for (int i = offset; i < offset + procLines; i++)
                for (int j = 0; j < originMatrix.W(); j++, arrIndex++)
                    matrArr[arrIndex] = originMatrix.GetValue(i, j);

            for (int i = 0; i < originVector.size(); i++)
                vectArr[i] = originVector[i];

            int lines = procLines;
            int columns = originMatrix.W();

            MPI_Send(&lines, 1, MPI_INT, procNumber, 0, MPI_COMM_WORLD);
            MPI_Send(&columns, 1, MPI_INT, procNumber, 1, MPI_COMM_WORLD);
            MPI_Send(matrArr, procLines * originMatrix.W(), MPI_INT, procNumber,
                     0, MPI_COMM_WORLD);
            MPI_Send(vectArr, originVector.size(), MPI_INT, procNumber, 0,
                     MPI_COMM_WORLD);
        }

        Matrix matr(mainProcLines, originMatrix.W());

        for (int i = 0; i < mainProcLines; i++)
            for (int j = 0; j < originMatrix.W(); j++)
                matr.SetValue(i, j, originMatrix.GetValue(i, j));

        vector<int> result(originMatrix.H());
        int resIndex = 0;

        vector<int> resPart = Sequence(matr, originVector);
        for (int i = 0; i < resPart.size(); i++, resIndex++)
            result[resIndex] = resPart[i];

        for (int i = 1; i < proccesors; i++) {
            int *vectArr = new int[procLines];
            MPI_Recv(vectArr, procLines, MPI_INT, i, 0, MPI_COMM_WORLD,
                     MPI_STATUSES_IGNORE);

            for (int j = 0; j < procLines; j++, resIndex++)
                result[resIndex] = vectArr[j];
        }

        return result;
    } else {
        int lines;
        int columns;
        MPI_Recv(&lines, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        MPI_Recv(&columns, 1, MPI_INT, 0, 1, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);

        int *matrArr = new int[lines * columns];
        int *vectArr = new int[columns];
        MPI_Recv(matrArr, lines * columns, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);
        MPI_Recv(vectArr, columns, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);

        Matrix matr = Matrix::ToMatrix(matrArr, lines, columns);
        vector<int> vect(columns);
        for (int i = 0; i < vect.size(); i++)
            vect[i] = vectArr[i];

        vector<int> resVect = Sequence(matr, vect);
        int *resArr = resVect.data();

        MPI_Send(resArr, lines, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    return vector<int>();
}

vector<int> GetRandomVector(int size) {
    std::random_device dev;
    std::mt19937 gen(dev());
    vector<int> result(size);

    for (int i = 0; i < result.size(); i++) {
        int random = -maxInt;
        random += gen() % (2 * maxInt);
        result[i] = random;
    }
    return result;
}
