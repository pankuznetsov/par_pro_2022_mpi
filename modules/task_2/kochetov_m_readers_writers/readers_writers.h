// Copyright 2023 Kochetov Maksim
#ifndef MODULES_TASK_2_KOCHETOV_M_READERS_WRITERS_READERS_WRITERS_H_
#define MODULES_TASK_2_KOCHETOV_M_READERS_WRITERS_READERS_WRITERS_H_
#include <string>
#include <vector>

void criticalOwner(int* criticalData, int requestsLimit);
void reader(int readsCount);
void writer(int writesCount);

#endif  // MODULES_TASK_2_KOCHETOV_M_READERS_WRITERS_READERS_WRITERS_H_
