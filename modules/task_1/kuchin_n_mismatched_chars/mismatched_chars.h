// Copyright 2022 Kuchin Nikita
#ifndef MODULES_TASK_1_KUCHIN_N_MISMATCHED_CHARS_MISMATCHED_CHARS_H_
#define MODULES_TASK_1_KUCHIN_N_MISMATCHED_CHARS_MISMATCHED_CHARS_H_

#include <string>
#include <utility>

void stringGen(char* const& str, int size, int token);
std::string genstring1(const int size, const int token);
int result(const std::string& str1, const std::string& str2, const int size);
int straightsum(const std::string& str1, const std::string& str2,
                const int size, int from, int to);

#endif  // MODULES_TASK_1_KUCHIN_N_MISMATCHED_CHARS_MISMATCHED_CHARS_H_
