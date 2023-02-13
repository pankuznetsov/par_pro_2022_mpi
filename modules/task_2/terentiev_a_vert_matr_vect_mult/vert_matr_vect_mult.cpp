// Copyright 2023 Alexander Terentiev
#include "../../../modules/task_2/terentiev_a_vert_matr_vect_mult/vert_matr_vect_mult.h"

Matrix::Matrix(int n, int m) : data(n * m) {
    str = n;
    col = m;
}

void Matrix::Randomize() {
    std::random_device dev;
    std::mt19937 gen(dev());

    for (int i = 0; i < data.size(); i++) {
        bool signMinus = gen() % 2;
        int randomElem = gen() % maxValue;
        if (signMinus)
            randomElem *= -1;

        data[i] = randomElem;
    }
}

int Matrix::Str() const { return str; }

int Matrix::Col() const { return col; }

int Matrix::Get(int i, int j) const {
    int index = i * col + j;
    return data[index];
}

void Matrix::Set(int i, int j, int value) {
    int index = i * col + j;
    data[index] = value;
}

void Matrix::Print() {
    cout << "\nMatrix:\n";
    for (int i = 0; i < Str(); i++) {
        for (int j = 0; j < Col(); j++) {
            cout << Get(i, j) << '\t';
        }
        cout << '\n';
    }
}

vector<int> MultipleSequential(const Matrix &matr, const vector<int> &vect) {
    vector<int> res(matr.Str());

    for (int i = 0; i < matr.Str(); i++) {
        int elemRes = 0;
        for (int j = 0; j < matr.Col(); j++)
            elemRes += matr.Get(i, j) * vect[j];

        res[i] = elemRes;
    }

    return res;
}

vector<int> MultipleParallel(const Matrix &matr, const vector<int> &vect) {
    int proc_count;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {  // main process
        int cols_for_proc = matr.Col() / proc_count;
        int cols_remainder = matr.Col() % proc_count;

        for (int proc_index = 1; proc_index < proc_count; proc_index++) {
            int col_offset = (cols_for_proc + cols_remainder) +
                             cols_for_proc * (proc_index - 1);

            int cols_send = cols_for_proc;
            int str_send = matr.Str();
            int *matr_part = new int[cols_for_proc * matr.Str()];
            int *vect_part = new int[cols_for_proc];

            int k = 0;
            int v = 0;
            for (int j = col_offset; j < col_offset + cols_for_proc; j++, v++) {
                for (int i = 0; i < matr.Str(); i++, k++) {
                    matr_part[k] = matr.Get(i, j);
                }
                vect_part[v] = vect[j];
            }

            // send data
            MPI_Send(&cols_send, 1, MPI_INT, proc_index, 0, MPI_COMM_WORLD);
            MPI_Send(&str_send, 1, MPI_INT, proc_index, 1, MPI_COMM_WORLD);
            MPI_Send(matr_part, cols_for_proc * matr.Str(), MPI_INT, proc_index,
                     0, MPI_COMM_WORLD);
            MPI_Send(vect_part, cols_for_proc, MPI_INT, proc_index, 0,
                     MPI_COMM_WORLD);
        }

        // handle own part
        int local_cols = cols_for_proc + cols_remainder;
        Matrix local_matr(matr.Str(), local_cols);
        for (int i = 0; i < local_matr.Str(); i++)
            for (int j = 0; j < local_matr.Col(); j++)
                local_matr.Set(i, j, matr.Get(i, j));

        vector<int> local_vect(local_cols);
        for (int i = 0; i < local_cols; i++)
            local_vect[i] = vect[i];

        vector<int> res = MultipleSequential(local_matr, local_vect);

        // collect data
        for (int i = 1; i < proc_count; i++) {
            int *accept = new int[res.size()];
            MPI_Recv(accept, res.size(), MPI_INT, i, 0, MPI_COMM_WORLD,
                     MPI_STATUSES_IGNORE);

            for (int j = 0; j < res.size(); j++)
                res[j] += accept[j];
        }

        return res;

    } else {  // other process
        int col = 0, str = 0;
        MPI_Recv(&col, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        MPI_Recv(&str, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        int *matr_part = new int[col * str];
        int *vect_part = new int[str];

        MPI_Recv(matr_part, col * str, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);
        MPI_Recv(vect_part, col, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);

        Matrix local_matr(str, col);
        for (int j = 0; j < col; j++)
            for (int i = 0; i < str; i++)
                local_matr.Set(i, j, matr_part[j * str + i]);

        vector<int> local_vect(col);
        for (int i = 0; i < col; i++)
            local_vect[i] = vect_part[i];

        vector<int> res;
        res = MultipleSequential(local_matr, local_vect);

        int *res_data = new int[res.size()];
        for (int i = 0; i < res.size(); i++)
            res_data[i] = res[i];

        MPI_Send(res_data, res.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    return vector<int>(1);
}

vector<int> RandomVector(int size) {
    vector<int> res(size);
    std::random_device dev;
    std::mt19937 gen(dev());

    for (int i = 0; i < res.size(); i++) {
        bool signMinus = gen() % 2;
        int randomElem = gen() % maxValue;
        if (signMinus)
            randomElem *= -1;

        res[i] = randomElem;
    }

    return res;
}

void PrintVector(vector<int> vect) {
    cout << "\nVector: ";
    for (int i = 0; i < vect.size(); i++)
        cout << vect[i] << ' ';
}
