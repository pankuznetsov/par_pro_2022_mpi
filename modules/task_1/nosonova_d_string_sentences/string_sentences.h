// Copyright 2022 Nosonova Darina
#ifndef MODULES_TASK_1_NOSONOVA_D_STRING_SENTENCES_STRING_SENTENCES_H_
#define MODULES_TASK_1_NOSONOVA_D_STRING_SENTENCES_STRING_SENTENCES_H_

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

char *GetRandomString(int size, int sentencesSize);

int GetSentencesQuantitySequence(char *str, int size, double *t);

int GetSentencesQuantityParallel(char *str, int size, double *t);

#endif  // MODULES_TASK_1_NOSONOVA_D_STRING_SENTENCES_STRING_SENTENCES_H_
