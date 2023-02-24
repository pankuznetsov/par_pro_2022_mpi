// Copyright 2022 Sukharev Artem

#ifndef MODULES_TASK_1_SUKHAREV_A_COUNT_LETTERS_IN_STRING_COUNT_LETTERS_IN_STRING_H_
#define MODULES_TASK_1_SUKHAREV_A_COUNT_LETTERS_IN_STRING_COUNT_LETTERS_IN_STRING_H_

#include <string>

std::string getRandomString(int size);

char getRandomLetter();

int countLettersSequential(const std::string& localString, const char letter);

int countLettersParallel(const std::string& globalString, char letter);

#endif  // MODULES_TASK_1_SUKHAREV_A_COUNT_LETTERS_IN_STRING_COUNT_LETTERS_IN_STRING_H_
