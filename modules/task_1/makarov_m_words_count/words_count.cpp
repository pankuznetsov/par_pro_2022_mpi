// Copyright 2023 Makarov Mikhail
#include "../../modules/task_1/makarov_m_words_count/words_count.h"

#include <mpi.h>

#include <iostream>
#include <random>
#include <string>
#include <vector>

/* Creates random string */
std::string getRandStr(int length) {
    std::random_device dev;
    std::mt19937 engine(dev());
    std::uniform_int_distribution<int> distr1(0, 9);
    std::uniform_int_distribution<int> distr2(0, 25);

    std::string str(length, ' ');

    const std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < length; i++) {
        int randRes1 = distr1(engine);
        if (randRes1 <= 1) {
            str[i] = ' ';
        } else {
            int randRes2 = distr2(engine);
            str[i] = alphabet[randRes2];
        }
    }

    return str;
}

void printIntArr(int array[], int size) {
    std::cout << "\n";
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << "\n";
}

int countWordsSequential(std::string str, int length) {
    int totalWords = 0;

    bool prevWasSpace = true;

    for (int i = 0; i < length; i++) {
        if (str[i] == ' ' && !prevWasSpace) {
            prevWasSpace = true;
        }
        if (str[i] != ' ' && prevWasSpace) {
            prevWasSpace = false;
            totalWords++;
        }
    }

    return totalWords;
}

int countWordsParallel(std::string str, int length) {
    int total_procs, wrank;
    MPI_Comm_size(MPI_COMM_WORLD, &total_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &wrank);

    // Every useful process gets minimum 3 symbols of str
    const int useful_procs_cnt = std::min(total_procs, length / 3);
    const bool is_proc_useful = wrank < useful_procs_cnt;
    const bool is_proc_root = wrank == 0;

    // Defines custom communicator
    int color = is_proc_useful ? 1 : 2;
    MPI_Comm MY_COMM;
    MPI_Comm_split(MPI_COMM_WORLD, color, wrank, &MY_COMM);
    if (color != 1) return 0;
    int mrank;
    MPI_Comm_rank(MY_COMM, &mrank);

    const int usual_symb_cnt = length / useful_procs_cnt;
    const int lost_symb_cnt = length % useful_procs_cnt;

    int localResult = 0;
    int hasStartSpace = 0;
    int hasEndSpace = 0;

    if (is_proc_root) {
        localResult =
            countWordsSequential(str, (usual_symb_cnt + lost_symb_cnt));
        hasStartSpace = (str[0] == ' ') ? 1 : 0;
        hasEndSpace =
            (str[(usual_symb_cnt + lost_symb_cnt) - 1] == ' ') ? 1 : 0;

        // Sending strpart to others
        auto pStart = str.c_str();

        for (int urank = 1; urank < useful_procs_cnt; urank++) {
            const char* buf = pStart + (usual_symb_cnt + lost_symb_cnt) +
                              (urank - 1) * usual_symb_cnt;
            MPI_Send(buf, usual_symb_cnt, MPI_CHAR, urank, 0, MY_COMM);
        }
    } else {
        // Receiving str part from MY_ROOT
        std::vector<char> pStrPart(usual_symb_cnt + 1);

        MPI_Status s;
        MPI_Recv(pStrPart.data(), usual_symb_cnt, MPI_CHAR, MY_ROOT, 0, MY_COMM, &s);

        pStrPart[usual_symb_cnt] = '\0';

        std::string strPart(pStrPart.data());

        localResult = countWordsSequential(strPart, usual_symb_cnt);
        hasStartSpace = (strPart[0] == ' ') ? 1 : 0;
        hasEndSpace = (strPart[usual_symb_cnt - 1] == ' ') ? 1 : 0;
    }

    int locResArr[] = {hasStartSpace, localResult, hasEndSpace};

    /* std::cout << "rank = " << wrank << ", resLocStr: ";
    for (int i = 0; i < 3; i++) {
        std::cout << locResArr[i] << ' ';
    }
    std::cout << '\n'; */

    std::vector<int> results(3 * useful_procs_cnt);

    MPI_Gather(&locResArr, 3, MPI_INT, results.data(), 3, MPI_INT, MY_ROOT, MY_COMM);

    if (is_proc_root) {
        // printIntArr(results, 3 * useful_procs_cnt);

        int wordsCount = results[1];

        for (int i = 3; i < 3 * useful_procs_cnt - 2; i += 3) {
            wordsCount += results[i + 1];

            int prevEndHasSpace = results[i - 1];
            int curStartHasSpace = results[i];

            if (!prevEndHasSpace && !curStartHasSpace) {
                wordsCount--;
            }
        }
        return wordsCount;
    }

    return 0;
}
