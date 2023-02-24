// Copyright 2022 Yarakhtin Anton
#ifndef MODULES_TASK_1_YARAKHTIN_A_COUNT_OF_WORDS_COUNT_OF_WORDS_H_
#define MODULES_TASK_1_YARAKHTIN_A_COUNT_OF_WORDS_COUNT_OF_WORDS_H_

#include <vector>
#include <string>

std::string getRandomString(int size);
int getWordsCountParallel(const std::string& str, int size);
int getWordsCountFragment(const std::string& str, int size);
int getWordsCountSequentially(const std::string& str, int size);

#endif  // MODULES_TASK_1_YARAKHTIN_A_COUNT_OF_WORDS_COUNT_OF_WORDS_H_
