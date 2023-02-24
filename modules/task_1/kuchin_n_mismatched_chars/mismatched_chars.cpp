// Copyright 2022 Kuchin Nikita

#include "../../../modules/task_1/kuchin_n_mismatched_chars/mismatched_chars.h"

#include <iostream>
#include <random>
#include <vector>

#include "./mpi.h"

void stringGen(char* const& str, int size, int token) {
    std::random_device dev;
    std::mt19937 gen(dev());
    switch (token) {
        case 1:
            for (int i = 0; i < size; i++) {
                str[i] = (gen() % 78) + 49;
            }
            break;
        case 2:
            for (int i = 0; i < size; i++) {
                str[i] = (gen() % 78) + 49;
            }
            break;
    }
}

std::string genstring1(const int size, const int token) {
    std::string alpha =
        "0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
    std::string str;
    std::random_device dev;
    std::mt19937 gen(dev());
    switch (token) {
        case 1:
            for (int i = 0; i < size; i++) {
                str[i] = alpha[(gen() % 63)];
            }
            break;
        case 2:
            for (int i = 0; i < size; i++) {
                str[i] = alpha[(gen() % 62)];
            }
            break;
    }
    return str;
}
int straightsum(const std::string& str1, const std::string& str2,
                const int size, int from, int to) {
    int sum = 0;
    for (int i = from; i < to; i++) {
        if (str1[i] != str2[i]) {
            sum++;
        }
    }
    return sum;
}

int result(const std::string& str1, const std::string& str2, const int size) {
    int rank, csize;
    int straighsum = 0, finalsum = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &csize);
    MPI_Status st;

    int k = size / csize;
    int i1 = k * rank;
    int i2 = k * (rank + 1);
    if (rank == csize - 1) {
        i2 = size;
    }
    straighsum = straightsum(str1, str2, size, i1, i2);
    MPI_Reduce(&straighsum, &finalsum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    return finalsum;
}
