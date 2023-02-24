// Copyright 2022 Tolkachev Nikita
#include <mpi.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_3/tolkachev_n_radix_batcher_int/radix_batcher_int.h"

vector<int> RandomVec(int n) {
    vector<int> mas;
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<int> dist(0, 100);
    for (int i = 0; i < n; i++) {
        int x = dist(gen);
        mas.push_back(x);
    }
    return mas;
}

void compexch(int* ap, int* bp) {
    int& a = *ap;
    int& b = *bp;
    if (a > b) {
        swap(a, b);
    }
}

void BatchMerge(vector<int>* arr, int n, int left, int right) {
    int step = right * 2;
    if (step >= n) {
        compexch(&(*arr)[left], &(*arr)[left + right]);
    } else {
        BatchMerge(arr, n, left, step);
        BatchMerge(arr, n, left + right, step);
        int i = left + right;
        while (i < left + n - right) {
            compexch(&(*arr)[i], &(*arr)[i + right]);
            i += step;
        }
    }
}

vector<int> merge(vector<vector<int>> all_vec) {
    while (all_vec.size() > 1) {
        for (int i = 0; i < all_vec.size(); i++) {
            all_vec[i].insert(all_vec[i].end(), all_vec[i + 1].begin(),
                all_vec[i + 1].end());
            BatchMerge(&all_vec[i], all_vec[i].size(), 0, 1);
            all_vec.erase(all_vec.begin() + i);
        }
    }
    return all_vec[0];
}

int MaxCategoryNumber(int n) {
    int count = 1;
    while (n = n / 10) count++;
    count--;
    return count;
}

void intSortCat(vector<int>* arr, int cat) {
    auto buffer = vector<vector<int>>(20);
    for (int i = 0; i < arr->size(); i++) {
        int temp = (*arr)[i];
        for (int j = 0; j < cat; j++) temp = temp / 10;
        int categoryValue = temp % 10;
        buffer[categoryValue + 9].push_back((*arr)[i]);
    }
    arr->resize(0);
    for (int i = 0; i < buffer.size(); i++) {
        for (int j = 0; j < buffer[i].size(); j++) {
            arr->push_back(buffer[i][j]);
        }
    }
}

void intSort(vector<int>* arr) {
    int maxCat = 0;
    for (int i = 0; i < arr->size(); i++) {
        if (MaxCategoryNumber((*arr)[i]) > maxCat) {
            maxCat = MaxCategoryNumber((*arr)[i]);
        }
    }
    for (int i = 0; i <= maxCat; i++) {
        intSortCat(arr, i);
    }
}

vector<int> ParSort(vector<int> arr, int size) {
    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<int> buffer = arr;
    int loc_size = size / comm_size;
    vector<int> loc(loc_size);
    vector<int> res;

    MPI_Scatter(buffer.data(), loc_size, MPI_INT, loc.data(), loc_size, MPI_INT,
        0, MPI_COMM_WORLD);
    intSort(&loc);
    MPI_Gather(loc.data(), loc_size, MPI_INT, buffer.data(),
        loc_size, MPI_INT, 0,
        MPI_COMM_WORLD);
    if (rank == 0) {
        vector<vector<int>> all_vec;
        for (int i = 0; i < comm_size; i++) {
            auto x = vector<int>(buffer.begin() + i * loc_size,
                buffer.begin() + (i + 1) * loc_size);
            all_vec.push_back(x);
        }
        res = merge(all_vec);
    }
    return res;
}
