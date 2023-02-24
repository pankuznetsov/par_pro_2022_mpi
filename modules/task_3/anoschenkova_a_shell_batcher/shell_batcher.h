// Copyright 2023 Anoschenkova Anna
#ifndef MODULES_TASK_3_ANOSCHENKOVA_A_SHELL_BATCHER_SHELL_BATCHER_H_
#define MODULES_TASK_3_ANOSCHENKOVA_A_SHELL_BATCHER_SHELL_BATCHER_H_

#include <vector>

std::vector<int> getRandomVector(int size);
std::vector<int> shellSort(const std::vector<int>& vec);
std::vector<int> evenFunc(const std::vector<int>& vec1,
                          const std::vector<int>& vec2);
std::vector<int> oddFunc(const std::vector<int>& vec1,
                         const std::vector<int>& vec2);
std::vector<int> transposition(std::vector<int> vec, int even_size,
                               int odd_size);
std::vector<int> merge(std::vector<int> vec, int even_size, int odd_size);
std::vector<int> permutation(std::vector<int> vec);

std::vector<int> batcherPar(std::vector<int> global_vec, int size_vec);
#endif  // MODULES_TASK_3_ANOSCHENKOVA_A_SHELL_BATCHER_SHELL_BATCHER_H_
