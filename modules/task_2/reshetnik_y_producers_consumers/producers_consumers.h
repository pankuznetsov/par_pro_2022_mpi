// Copyright 2022 Reshetnik Yana
#ifndef MODULES_TASK_2_RESHETNIK_Y_PRODUCERS_CONSUMERS_PRODUCERS_CONSUMERS_H_
#define MODULES_TASK_2_RESHETNIK_Y_PRODUCERS_CONSUMERS_PRODUCERS_CONSUMERS_H_

#include <vector>
#include <string>

#define PRODUCER 1
#define CONSUMER 2
#define INFO 3

int generate_resource();
void delay(double seconds);
int task(int buffer_size);
void produce(double time);
void consume(double time);

#endif  // MODULES_TASK_2_RESHETNIK_Y_PRODUCERS_CONSUMERS_PRODUCERS_CONSUMERS_H_
