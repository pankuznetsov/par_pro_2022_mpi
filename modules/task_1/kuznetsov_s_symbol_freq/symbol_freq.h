// Copyright 2023 Kuznetsov Sergey
#ifndef SYMBOL_FREQ_H_
#define SYMBOL_FREQ_H_

#include <vector>
#include <string>

std::string randomString(int length);
int symbolFrequencyParallel(std::string& str, char symbol);
int symbolFrequencySequential(std::string& str, char symbol);

#endif  // SYMBOL_FREQ_H_
