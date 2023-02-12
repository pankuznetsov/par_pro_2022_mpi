// Copyright 2023 Bezrukov Aleksandr
#include "../../../modules/task_3/bezrukov_a_radix_sort/radix_sort.h"
#include <mpi.h>
#include <algorithm>
#include <iostream>
#include <random>

std::vector<double> genRandomVector(int len) {
    std::random_device randDevice;
    std::mt19937 genEngine(randDevice());
    std::uniform_real_distribution<> doubleDistribution(-100.0, 100.0);
    std::vector<double> targetVec(len);
    for (int i = 0; i < len; ++i) targetVec[i] = doubleDistribution(genEngine);
    return targetVec;
}

void printVector(const std::vector<int> vec) {
    for (size_t k = 0; k < vec.size(); k++) std::cout << vec[k] << ' ';
}

void radixSortSequential(std::vector<double>* array) {
    int size = static_cast<int>((*array).size());
    long long* a = new long long[size];
    long long* a2 = new long long[size];
    int* buck = new int[512];
    bool p;
    int b;

    for (int i = 0; i < size; i++) a[i] = *(long long*)&(*array)[i];

    for (int shift = 0; shift < 63; shift += 9) {
        for (int i = 0; i <= 0x1ff; i++) buck[i] = 0;
        for (int i = 0; i < size; i++) buck[a[i] >> shift & 0x1ff]++;
        for (int i = 0x1ff; i > 0; i--) buck[i - 1] += buck[i];
        for (int i = size - 1; i >= 0; i--)
            a2[--buck[a[i] >> shift & 0x1ff]] = a[i];
        long long* tmp = a;
        a = a2;
        a2 = tmp;
    }

    buck[0] = 0;
    buck[1] = 0;
    for (int i = 0; i < size; i++) buck[a[i] >> 63 & 1]++;

    for (int i = size - 1; i >= 0; i--) {
        p = a[i] >> 63 & 1;
        b = --buck[p];
        (*array)[b - p * (b + b - size + 1)] = *(double*)&a[i];
    }

    delete[] buck;
    delete[] a;
    delete[] a2;
}

void radixSortParallel(std::vector<double>* globVec, int len) {
    int comm_size, cur_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &cur_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    int maxP = len / 2;
    int usefulPCount = std::min(comm_size, maxP);

    int color = (cur_rank < usefulPCount) ? 0 : 1;
    MPI_Comm CUSTOM_COMM;
    MPI_Comm_split(MPI_COMM_WORLD, color, cur_rank, &CUSTOM_COMM);
    if (color != 0) return;

    const int countPerP = len / usefulPCount;
    const int remains = len % usefulPCount;

    if (cur_rank == 0) {
        for (int pRank = 1; pRank < usefulPCount; pRank++)
            MPI_Send(globVec->data() + pRank * countPerP + remains, countPerP,
                     MPI_DOUBLE, pRank, 0, CUSTOM_COMM);
    }

    std::vector<double> localVector(countPerP);
    if (cur_rank == 0) {
        localVector = std::vector<double>(
            globVec->begin(), globVec->begin() + countPerP + remains);
    } else {
        MPI_Status stat;
        MPI_Recv(localVector.data(), countPerP, MPI_DOUBLE, 0, 0, CUSTOM_COMM,
                 &stat);
    }

    // Sorts local parts
    radixSortSequential(&localVector);

    std::vector<double> globVec2(len);
    MPI_Gather(localVector.data(), countPerP, MPI_DOUBLE, globVec2.data(),
               countPerP, MPI_DOUBLE, 0, CUSTOM_COMM);

    if (cur_rank == 0) {
        for (int i = 0; i < remains; i++)
            globVec2[countPerP * usefulPCount + i] = localVector[countPerP + i];

        std::vector<int> posArr(usefulPCount + 1);
        if (remains == 0) posArr[usefulPCount] = -1;

        for (int i = 0; i < len; i++) {
            int maxPartIdx = -1;

            for (int j = 0; j < usefulPCount + 1; j++) {
                if (posArr[j] >= 0) {
                    maxPartIdx = j;
                    break;
                }
            }

            for (int j = maxPartIdx + 1; j < usefulPCount + 1; j++) {
                if (posArr[j] >= 0 &&
                    globVec2[countPerP * j + posArr[j]] >
                        globVec2[countPerP * maxPartIdx + posArr[maxPartIdx]]) {
                    maxPartIdx = j;
                }
            }

            if (maxPartIdx < 0) break;

            (*globVec)[i] =
                globVec2[countPerP * maxPartIdx + posArr[maxPartIdx]];
            posArr[maxPartIdx]++;

            if (maxPartIdx != usefulPCount && posArr[maxPartIdx] == countPerP)
                posArr[maxPartIdx] = -1;
            else if (maxPartIdx == usefulPCount &&
                     posArr[maxPartIdx] == remains)
                posArr[maxPartIdx] = -1;
        }
    }
}
