// Copyright 2022 Tsvetkov
#include <mpi.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <ctime>
#include <random>
#include <queue>
#include <iostream>
#include "../../../modules/task_2/tsvetkov_a_producers_consumers/producers_consumers.h"

int main_proc(int kol_prod_mes) {
    int ProcNum, ProcRank, req, res, wait_consumer = -1;
    std::queue <int> product;
    std::queue <int> consumers;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Status condition;
    MPI_Request request;
    while (kol_prod_mes > 0) {
        MPI_Recv(&req, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &condition);
        if (condition.MPI_TAG == want_to_get) {
            if (wait_consumer == -1) {
                if (product.empty() == true) {
                wait_consumer = condition.MPI_SOURCE;
                } else {
                    wait_consumer = condition.MPI_SOURCE;
                    res = product.front();
                    product.pop();
                    std::cout << "Process number: " << wait_consumer << " get elem: " << res << "\n";
                    MPI_Send(&res, 1, MPI_INT, wait_consumer, can_to_get, MPI_COMM_WORLD);
                    kol_prod_mes--;
                    if (consumers.empty() == true) {
                        wait_consumer = -1;
                    } else {
                        std::cout << "Process nasasas: ";
                        wait_consumer = consumers.front();
                        consumers.pop();
                    }
                }
            } else {
                consumers.push(condition.MPI_SOURCE);
            }
        }
        if (condition.MPI_TAG == add_elem) {
            kol_prod_mes--;
            product.push(req);
            std::cout << "Process number: " << condition.MPI_SOURCE << " add elem: " << req << "\n";
            if (wait_consumer != -1) {
                res = product.front();
                product.pop();
                std::cout << "Process number: " << wait_consumer << " get elem: " << res << "\n";
                MPI_Send(&res, 1, MPI_INT, wait_consumer, can_to_get, MPI_COMM_WORLD);
                kol_prod_mes--;
                if (consumers.empty() == true) {
                    wait_consumer = -1;
                } else {
                    wait_consumer = consumers.front();
                    consumers.pop();
                }
            }
        }
    }
    return product.size();
}

int produce() {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Send(&rank, 1, MPI_INT, 0, add_elem, MPI_COMM_WORLD);
  return 0;
}

int consume() {
  int rank, res;
  MPI_Status condition;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Send(&rank, 1, MPI_INT, 0, want_to_get, MPI_COMM_WORLD);
  MPI_Recv(&res, 1, MPI_INT, 0, can_to_get, MPI_COMM_WORLD, &condition);
  return 0;
}
