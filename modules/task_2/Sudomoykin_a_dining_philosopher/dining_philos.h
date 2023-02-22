// Copyright 2023 Andrey Sudomoykin

#ifndef MODULES_TASK_2_SUDOMOYKIN_A_DINING_PHILOSOPHER_DINING_PHILOS_H_
#define MODULES_TASK_2_SUDOMOYKIN_A_DINING_PHILOSOPHER_DINING_PHILOS_H_

#include <mpi.h>

#include <stdio.h>

#include <chrono>

#include <thread>

#include <iostream>

#include <random>


#define THINK 0
#define HUNGRY 1
#define EAT 2
#define IN 4
#define OUT 1
#define KEY 3
#define MAX_COUNT 5

void start(int rank, int size, int run);

#endif  // MODULES_TASK_2_SUDOMOYKIN_A_DINING_PHILOSOPHER_DINING_PHILOS_H_
