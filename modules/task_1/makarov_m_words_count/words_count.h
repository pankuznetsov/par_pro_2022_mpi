// Copyright 2023 Makarov Mikhail
#ifndef MODULES_TASK_1_MAKAROV_M_WORDS_COUNT_WORDS_COUNT_H_
#define MODULES_TASK_1_MAKAROV_M_WORDS_COUNT_WORDS_COUNT_H_

#include <iostream>
#include <string>
#include <vector>
#include <random>

#define MY_ROOT (0)

std::string getRandStr(int length);
void printIntArr(int array[], int size);

int countWordsSequential(std::string str, int length);
int countWordsParallel(std::string str, int length);

#endif  // MODULES_TASK_1_MAKAROV_M_WORDS_COUNT_WORDS_COUNT_H_
