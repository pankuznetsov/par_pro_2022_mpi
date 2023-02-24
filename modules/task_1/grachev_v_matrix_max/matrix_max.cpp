// Copyright 2022 Grachev Valentin
#include "../../../modules/task_1/grachev_v_matrix_max/matrix_max.h"

TMatrix::TMatrix(int m, int n) {
    str_count = m;
    col_count = n;
    arr = new double[m * n];
}

TMatrix::TMatrix(const TMatrix &other) {
    str_count = other.str_count;
    col_count = other.col_count;
    arr = new double[str_count * col_count];
    for (int i = 0; i < str_count * col_count; i++) {
        arr[i] = other.arr[i];
    }
}

TMatrix &TMatrix::operator=(const TMatrix &other) {
    if (this == &other)
        return *this;
    str_count = other.str_count;
    col_count = other.col_count;
    if (arr)
        delete[] arr;
    arr = new double[str_count * col_count];
    for (int i = 0; i < str_count * col_count; i++) {
        arr[i] = other.arr[i];
    }
    return *this;
}

TMatrix::~TMatrix() { delete[] arr; }

void TMatrix::SetElement(int i, int j, double value) {
    int offset = i * col_count;
    arr[offset + j] = value;
}

double TMatrix::GetElement(int i, int j) const {
    int offset = i * col_count;
    return arr[offset + j];
}

void TMatrix::FillRandom(int min, int max) {
    std::random_device dev;
    std::mt19937 gen(dev());
    for (size_t i = 0; i < str_count * col_count; i++) {
        double random = min + (gen() % (max - min));
        arr[i] = random;
    }
}

int TMatrix::GetStringCount() const { return str_count; }

int TMatrix::GetColumnCount() const { return col_count; }

double *TMatrix::GetStringValues(int str_number) const {
    double *res = new double[col_count];
    int offset = str_number * col_count;

    for (int i = 0; i < col_count; i++) {
        res[i] = arr[offset + i];
    }

    return res;
}

double *TMatrix::GetColumnValues(int col_number) const {
    double *res = new double[str_count];

    for (int i = 0; i < str_count; i++) {
        int offset = col_count * i;
        res[i] = arr[offset + col_number];
    }

    return res;
}

void TMatrix::PrintElements() const {
    for (int i = 0; i < str_count * col_count; i++) {
        std::cout << arr[i] << ' ';
    }
}

double GetMax(double *arr, int size) {
    double max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

double GetMatrixMaxLinear(const TMatrix &matrix, double *time) {
    double start_time = clock();
    double result = matrix.GetElement(0, 0);
    for (int i = 0; i < matrix.GetStringCount(); i++) {
        for (int j = 0; j < matrix.GetColumnCount(); j++) {
            result = std::max(result, matrix.GetElement(i, j));
        }
    }

    double end_time = clock();
    *time = end_time - start_time;
    *time /= 1000;
    return result;
}

void GetMatrixMaxParallel(const TMatrix &matrix, double *time, double *result) {
    int process_quantity;
    MPI_Comm_size(MPI_COMM_WORLD, &process_quantity);

    int rank_this_process;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_this_process);

    double local_max, global_max, start_time, end_time;

    if (rank_this_process == 0) {
        start_time = MPI_Wtime();
        for (int i = 0; i < process_quantity; i++) {
            std::vector<double> vect;
            if (matrix.GetStringCount() >= matrix.GetColumnCount()) {
                for (int j = i; j < matrix.GetStringCount();
                     j += process_quantity) {
                    double *stringValues = matrix.GetStringValues(j);
                    int stringSize = matrix.GetColumnCount();
                    for (int k = 0; k < stringSize; k++) {
                        vect.push_back(stringValues[k]);
                    }
                }
            } else {
                for (int j = i; j < matrix.GetColumnCount();
                     j += process_quantity) {
                    double *columnValues = matrix.GetColumnValues(j);
                    int columnSize = matrix.GetStringCount();
                    for (int k = 0; k < columnSize; k++) {
                        vect.push_back(columnValues[k]);
                    }
                }
            }
            double *message = vect.data();
            int size = vect.size();
            if (i == 0) {
                local_max = GetMax(message, vect.size());
            } else {
                MPI_Send(&size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(message, vect.size(), MPI_DOUBLE, i, 0,
                         MPI_COMM_WORLD);
            }
        }
    } else {
        int size;
        MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        double *arr = new double[size];
        MPI_Recv(arr, size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);
        local_max = GetMax(arr, size);
    }

    MPI_Reduce(&local_max, &global_max, 1, MPI_DOUBLE, MPI_MAX, 0,
               MPI_COMM_WORLD);
    if (rank_this_process == 0) {
        *result = global_max;
        end_time = MPI_Wtime();
        *time = end_time - start_time;
    }
}
