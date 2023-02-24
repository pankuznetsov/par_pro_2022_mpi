// Copyright 2023 Kuleva Anna
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/kuleva_a_check_lex_ordering/check_lex_ordering.h"

std::string getRandomString(const int strLen) {
    std::string valid_chars =
        "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::random_device dev;
    std::mt19937 gen(dev());
    std::string str;
    if (strLen <= 0) throw "Wrong length";
    str.reserve(strLen);
    for (int i = 0; i < strLen; i++) {
        str.push_back(
            valid_chars[gen() % valid_chars.length()]);
    }
    return str;
}

int getSequentialOperations(std::string str_inp1, std::string str_inp2) {
    const size_t len1 = str_inp1.length();
    const size_t len2 = str_inp2.length();
    if (str_inp1 == str_inp2) {
        return 0;
    } else {
        for (int i = 0; i < std::min(len1, len2); i++) {
            if (str_inp1[i] < str_inp2[i]) return -1;
            if (str_inp1[i] > str_inp2[i]) return 1;
        }
        if (str_inp1.length() > str_inp2.length()) {
            return 1;
        } else if (str_inp1.length() < str_inp2.length()) {
            return -1;
        }
    }
    return 0;
}

int getParallelOperations(std::string global_str_inp1, std::string global_str_inp2,
                        size_t global_len) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    const size_t delta = global_len / size;
    const size_t remainder = global_len % size;

    std::string loc_str1, loc_str2;
    loc_str1.reserve(delta + remainder);
    loc_str2.reserve(delta + remainder);

    std::string rem_str1, rem_str2;
    rem_str1.reserve(remainder);
    rem_str2.reserve(remainder);

    if (rank == 0) {
        for (int proc = 1; proc < size; proc++) {
            char* buf1, * buf2;
            buf1 = new char[delta + remainder];
            buf2 = new char[delta + remainder];
            for (int i = 0; i < delta; i++) {
                buf1[i] = global_str_inp1[proc * delta + i];
                buf2[i] = global_str_inp2[proc * delta + i];
            }
            for (int i = global_len - remainder; i < global_len; i++) {
                rem_str1.push_back(global_str_inp1[i]);
                rem_str2.push_back(global_str_inp2[i]);
            }
            MPI_Send(buf1, static_cast<int>(delta),
                MPI_CHAR, proc, 0, MPI_COMM_WORLD);
            MPI_Send(buf2, static_cast<int>(delta),
                MPI_CHAR, proc, 1, MPI_COMM_WORLD);
        }
    }

    if (rank != 0) {
        MPI_Status status;
        char* buf1, * buf2;
        buf1 = new char[delta + remainder];
        buf2 = new char[delta + remainder];
        MPI_Recv(buf1, static_cast<int>(delta),
            MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(buf2, static_cast<int>(delta),
            MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
        for (int i = 0; i < delta; i++) {
            loc_str1.push_back(buf1[i]);
            loc_str2.push_back(buf2[i]);
        }
    } else if (rank == 0) {
        for (int i = 0; i < delta + remainder; i++) {
            loc_str1.push_back(global_str_inp1[i]);
            loc_str2.push_back(global_str_inp2[i]);
        }
    }

    std::vector<int> glob_result(size, 0);
    int loc_result = getSequentialOperations(loc_str1, loc_str2);
    MPI_Gather(&loc_result, 1, MPI_INT, glob_result.data(), 1, MPI_INT, 0,
               MPI_COMM_WORLD);

    if (rank == 0) {
        for (const auto& result : glob_result) {
            if (result == 1) {
                return result;
            } else if (result == -1) {
                return result;
            }
        }
        int rem_res = getSequentialOperations(rem_str1, rem_str2);
        if (rem_res == 1) {
            return rem_res;
        } else if (rem_res == -1) {
            return rem_res;
        }
        if (global_str_inp1.length() > global_str_inp2.length()) {
            return 1;
        } else if (global_str_inp1.length() < global_str_inp2.length()) {
            return -1;
        }
        return 0;
    }
    return 0;
}
