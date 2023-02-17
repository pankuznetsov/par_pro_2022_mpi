// Copyright 2022 Kudryavtsev Nikita
#include <mpi.h>
#include <iostream>
#include <queue>
#include <random>
#include "../../../modules/task_2/kudryavtsev_n_sleeping_barber/sleeping_barber.h"

void expect(double minutes) {
    double time_now = MPI_Wtime();
    while (MPI_Wtime() - time_now < minutes) {
    }
}

void print_messages(int ID, const char* message_text, double num) {
    if (ID == 0)
        std::cout << "Barber said: " << message_text << ' ' << num << std::endl;
    else
        std::cout << "Client with number " << ID << ": " << message_text << ' ' << num << std::endl;
}

void print_messages(int ID, const char* message_text, int num) {
    if (ID == 0)
        std::cout << "Barber said: " << message_text << ' ' << num << std::endl;
    else
        std::cout << "Client with number " << ID << ": " << message_text << ' ' << num << std::endl;
}


void Barber(int size_of_queue) {
    std::queue<int> que;
    int client_complaints = 0;
    while (client_complaints != size_of_queue) {
        int is_waiting_clients = 0;
        int Free = 0;  // Barber is free now

        // Ask, is some waiting?
        MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &is_waiting_clients, MPI_STATUS_IGNORE);
        int client_id;
        while (is_waiting_clients) {
            MPI_Recv(&client_id, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            print_messages(0, "Hello, client with number:", client_id);
            if (static_cast<int>(que.size()) < size_of_queue) {
                que.push(client_id);
                Free = 1;
                MPI_Send(&Free, 1, MPI_INT, client_id, 0, MPI_COMM_WORLD);
            } else {
                Free = 0;
                MPI_Send(&Free, 1, MPI_INT, client_id, 0, MPI_COMM_WORLD);
            }
            // Ask, is some waiting again?
            MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &is_waiting_clients, MPI_STATUS_IGNORE);
        }

        if (!que.empty()) {
            client_id = que.front();
            que.pop();
            print_messages(0, "Begin hair cutting for number", client_id);
            int is_cutting_now = 1;
            MPI_Send(&is_cutting_now, 1, MPI_INT, client_id, 0, MPI_COMM_WORLD);
            expect(0.5);
            print_messages(0, "Finish hair cutting for number", client_id);
            is_cutting_now = 0;
            MPI_Send(&is_cutting_now, 1, MPI_INT, client_id, 0, MPI_COMM_WORLD);
            client_complaints++;
        }
    }
}

double TimeRandomizer() {
    std::random_device rand_machine;
    std::mt19937 generate(rand_machine());
    return (100.0 + static_cast<double>(generate() % 400u)) / 1000.0;
}

void Client(int ID) {
    double waiting_time = TimeRandomizer();
    print_messages(ID, "Waiting on BEGIN...", waiting_time);
    expect(waiting_time);
    int barber_is_cutting = 1;
    while (barber_is_cutting != 0) {
        MPI_Send(&ID, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        int barber_is_free = 0;
        MPI_Recv(&barber_is_free, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (barber_is_free) {
            MPI_Recv(&barber_is_free, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&barber_is_cutting, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            double waiting_time = TimeRandomizer();
            print_messages(ID, "Waiting for a Barber...", waiting_time);
            expect(waiting_time);
        }
    }
}

void load() {
    int Size, Rank;
    MPI_Comm_size(MPI_COMM_WORLD, &Size);
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank);
    if (Size < 2) {
        throw "Oooh, so little comm size";
    }
    if (Rank == 0)
        Barber(Size - 1);
    else
        Client(Rank);
}
