// Copyright 2022 Reshetnik Yana
#include <mpi.h>
#include <vector>
#include <random>
#include <list>
#include <algorithm>
#include "../../../modules/task_2/reshetnik_y_producers_consumers/producers_consumers.h"

int generate_resource() {
    static std::random_device ran_dev;
    static std::mt19937 gener(ran_dev());
    return gener();
}

void delay(double seconds) {
    double endtime = MPI_Wtime() + seconds;
    while (MPI_Wtime() < endtime) {
        continue;
    }
}

int task(int buffer_size) {
    MPI_Status status;
    int size;
    int type;
    int* buffer = new int[buffer_size];
    MPI_Request* requests = new int[buffer_size];
    std::vector<int> producers;
    std::vector<int> consumers;

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    size -= (size + 1) % 2;

    for (int i = 0; i < buffer_size; i++) {
        requests[i] = 0;
    }

    for (int i = 1; i < size; i++) {
        MPI_Recv(&type, 1, MPI_INT, i, INFO, MPI_COMM_WORLD, &status);
        if (type == CONSUMER) {
            consumers.push_back(type);
        } else {
            producers.push_back(type);
        }
    }

    int prod = std::min(buffer_size, static_cast<int>(producers.size()));

    for (int i = 0; i < prod; i++) {
        MPI_Irecv(&(buffer[i]), 1, MPI_INT, producers[i], PRODUCER, MPI_COMM_WORLD, &(requests[i]));
    }

    for (int i = 0; i < consumers.size(); i++) {
        int idx;
        MPI_Waitany(prod, requests, &idx, &status);
        MPI_Ssend(&(buffer[idx % buffer_size]), 1, MPI_INT, consumers[i], CONSUMER, MPI_COMM_WORLD);
        if (idx + buffer_size < producers.size()) {
            MPI_Irecv(&(buffer[idx % buffer_size]), 1, MPI_INT,
                producers[idx + buffer_size], PRODUCER, MPI_COMM_WORLD, &(requests[idx % buffer_size]));
        } else {
            requests[idx % buffer_size] = 0;
        }
    }

    int check = 0;
    for (int i = 0; i < buffer_size; i++) {
        check += requests[i];
    }

    delete[] buffer;
    delete[] requests;
    return check;
}

void produce(double time) {
    int type = PRODUCER;
    int resource;

    MPI_Send(&type, 1, MPI_INT, 0, INFO, MPI_COMM_WORLD);

    delay(time);
    resource = generate_resource();
    MPI_Send(&resource, 1, MPI_INT, 0, PRODUCER, MPI_COMM_WORLD);
}

void consume(double time) {
    MPI_Status status;
    int type = CONSUMER;
    int resource;

    MPI_Send(&type, 1, MPI_INT, 0, INFO, MPI_COMM_WORLD);

    delay(time);
    MPI_Recv(&resource, 1, MPI_INT, 0, CONSUMER, MPI_COMM_WORLD, &status);
}
