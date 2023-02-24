// Copyright 2023 Kuleva Anna
#ifndef MODULES_TASK_1_KULEVA_A_CHECK_LEX_ORDERING_CHECK_LEX_ORDERING_H_
#define MODULES_TASK_1_KULEVA_A_CHECK_LEX_ORDERING_CHECK_LEX_ORDERING_H_

#include <vector>
#include <string>

std::string getRandomString(const int strLen);
int getParallelOperations(std::string global_str_inp1, std::string global_str_inp2,
                        size_t global_len);
int getSequentialOperations(std::string str_inp1,
                        std::string str_inp2);

#endif  // MODULES_TASK_1_KULEVA_A_CHECK_LEX_ORDERING_CHECK_LEX_ORDERING_H_
