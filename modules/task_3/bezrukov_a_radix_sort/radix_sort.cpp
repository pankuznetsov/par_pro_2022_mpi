// Copyright 2023 Bezrukov Aleksandr
#include "../../../modules/task_3/bezrukov_a_radix_sort/radix_sort.h"
#include <mpi.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <random>

std::vector<double> genRandomVector(int len) {
    std::random_device randDevice;
    std::mt19937 genEngine(randDevice());
    std::uniform_real_distribution<> doubleDistribution(100.0, 300.0);
    std::vector<double> targetVec(len);
    for (int i = 0; i < len; ++i) targetVec[i] = doubleDistribution(genEngine);
    return targetVec;
}

void printVector(const std::vector<double>& vec) {
    for (size_t k = 0; k < vec.size(); k++) std::cout << vec[k] << ' ';
}

int leftOfThePoint(double number) {
    int radix = 0;
    while (number > 1) {
        number /= 10;
        radix++;
    }
    return radix;
}

int rightOfThePoint(double number) {
    std::ostringstream strs;
    strs << number;
    std::string str = strs.str();
    int size = str.size();
    int pos = str.find('.');
    int value = size - pos - 1;
    return value;
}

int getDigit(double number, int radix) {
    if (radix > 0) {
        double mask = pow(10, radix);
        double tmp = number / mask;
        return static_cast<int>(tmp) % 10;
    }
    return  static_cast<int>(number * pow(10, -radix)) % 10;
}

std::vector<double> sortByOneRadix(const std::vector<double>& vect, int rad) {
    std::vector<double> res;
    std::vector <std::vector<double>> radix(10);

    for (int i = 0; i < static_cast<int>(vect.size()); i++)
        radix[getDigit(vect[i], rad)].push_back(vect[i]);

    for (int i = 0; i < static_cast<int>(radix.size()); ++i)
        for (int j = 0; j < static_cast<int>(radix[i].size()); ++j)
            res.push_back(radix[i][j]);
    return res;
}

void radixSortSequential(std::vector<double>* array) {
    int size = (*array).size();
    int maxRadixNegativeZero = rightOfThePoint((*array)[0]);
    for (int i = 1; i < size; ++i) {
        int radixNegativeZero = 0;
        radixNegativeZero = rightOfThePoint((*array)[i]);
        if (radixNegativeZero > maxRadixNegativeZero)
            maxRadixNegativeZero = radixNegativeZero;
    }
    double max_elem = (*array)[0];
    for (int i = 1; i < size; i++) {
        if ((*array)[i] > max_elem)
            max_elem = (*array)[i];
    }
    int maxRadixPositiveZero = leftOfThePoint(max_elem);
    std::vector<double> result((*array));
    for (int i = -maxRadixNegativeZero; i <= maxRadixPositiveZero; i++)
        result = sortByOneRadix(result, i);

    for(int i = 0; i < size; i++)
        (*array)[i] = result[i];
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
            int minPartIdx = -1;

            for (int j = 0; j < usefulPCount + 1; j++) {
                if (posArr[j] >= 0) {
                    minPartIdx = j;
                    break;
                }
            }

            for (int j = minPartIdx + 1; j < usefulPCount + 1; j++) {
                if (posArr[j] >= 0 &&
                    globVec2[countPerP * j + posArr[j]] <
                        globVec2[countPerP * minPartIdx + posArr[minPartIdx]]) {
                    minPartIdx = j;
                }
            }

            if (minPartIdx < 0) break;

            (*globVec)[i] =
                globVec2[countPerP * minPartIdx + posArr[minPartIdx]];
            posArr[minPartIdx]++;

            if (minPartIdx != usefulPCount && posArr[minPartIdx] == countPerP)
                posArr[minPartIdx] = -1;
            else if (minPartIdx == usefulPCount &&
                     posArr[minPartIdx] == remains)
                posArr[minPartIdx] = -1;
        }
    }
}
