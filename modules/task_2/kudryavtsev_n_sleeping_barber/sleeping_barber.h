// Copyright 2022 Kudryavtsev Nikita
#ifndef MODULES_TASK_2_KUDRYAVTSEV_N_SLEEPING_BARBER_SLEEPING_BARBER_H_
#define MODULES_TASK_2_KUDRYAVTSEV_N_SLEEPING_BARBER_SLEEPING_BARBER_H_

void expect(double minutes);
void print_messages(int ID, const char* message_text, double num);
void print_messages(int ID, const char* message_text, int num);
void Barber(int size_of_queue);
void Client(int ID);
void load();
double TimeRandomizer();

#endif  // MODULES_TASK_2_KUDRYAVTSEV_N_SLEEPING_BARBER_SLEEPING_BARBER_H_

