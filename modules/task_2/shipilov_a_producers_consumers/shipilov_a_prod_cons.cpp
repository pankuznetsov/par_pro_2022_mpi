// Copyright 2022 Shipilov Artem
#include "../../../modules/task_2/shipilov_a_producers_consumers/shipilov_a_prod_cons.h"
#include <mpi.h>
#include <iostream>
#include <utility>
#include <initializer_list>
#include <queue>
#include <vector>
#include <random>
// #include <thread>

using std::cout;
using std::endl;
using std::pair;
using std::initializer_list;
using std::queue;
using std::vector;
using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;

// tags for messages (roles of processes)
#define PRODUCER 0
#define CONSUMER 1
#define FINISH   2

// data about producer/consumer request
struct request_t {
    int id;
    int rank;
    int value;
};

// buffer (std::queue) and related variables
struct buffer_t {
    queue<int> buf;
    int buf_size;
    bool empty;
    bool full;
    queue<request_t> p_queue;
    queue<request_t> c_queue;
    int free_cnt;

    // insert stuff into buffer (std::queue)
    void insert(int value) {
        request_t entry = p_queue.front();

        value = entry.value;
        empty = false;
        buf.push(value);
        if (buf.size() == buf_size)
            full = true;

        cout << "Producer " << entry.rank << " put value " << value
             << ", buffer size: " << buf.size() << endl;
        MPI_Ssend(&value, 1, MPI_INT, entry.rank, PRODUCER, MPI_COMM_WORLD);
        p_queue.pop();
        ++free_cnt;
    }

    // extract stuff from buffer (std::queue)
    int extract() {
        request_t entry = c_queue.front();

        full = false;
        int value = buf.front();
        buf.pop();
        if (buf.size() == 0)
            empty = true;

        cout << "Consumer " << entry.rank << " got value " << value
             << ", buffer size: " << buf.size() << endl;
        MPI_Ssend(&value, 1, MPI_INT, entry.rank, CONSUMER, MPI_COMM_WORLD);
        c_queue.pop();
        ++free_cnt;

        return value;
    }
};

void producers_consumers(int prod_num,
                         int prod_count,
                         int cons_num,
                         int cons_count,
                         int buffer_capacity) {
    int rank;
    int value;
    int response;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {  // master process
        int max_id = prod_num * (prod_count + 1) + cons_num * (cons_count + 1);
        int active_process_cnt = prod_num + cons_num;
        int recv_id = 0;
        buffer_t buf;
        MPI_Status status;
        queue<request_t> &prod_q = buf.p_queue;
        queue<request_t> &cons_q = buf.c_queue;

        buf.buf_size = buffer_capacity;
        buf.free_cnt = prod_num + cons_num;
        buf.full = false;
        buf.empty = true;
        while (active_process_cnt) {
            // Receive request and process it
            // if 'free_cnt' is 0 then there's no process to get message from
            if (recv_id < max_id && buf.free_cnt) {
                MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,
                        MPI_COMM_WORLD, &status);
                --buf.free_cnt;

                switch (status.MPI_TAG) {
                    // producer request goes into its queue
                    case PRODUCER:
                        buf.p_queue.push({recv_id, status.MPI_SOURCE, value});
                        break;
                    // consumer request goes into its queue
                    case CONSUMER:
                        buf.c_queue.push({recv_id, status.MPI_SOURCE, value});
                        break;
                    // finish request means that the process is done
                    case FINISH:
                        --active_process_cnt;
                        break;
                }
                ++recv_id;
            }

            // process requests from queues
            if (!(prod_q.empty() && cons_q.empty())) {
                // if only one queue isn't empty and buffer isn't full (empty),
                // definitely insert (extract)
                if (!prod_q.empty() && cons_q.empty() && !buf.full)
                    buf.insert(value);
                if (!cons_q.empty() && prod_q.empty() && !buf.empty)
                    value = buf.extract();
                if (!prod_q.empty() && !cons_q.empty()) {
                    if (buf.empty)
                        buf.insert(value);
                    else if (buf.full)
                        value = buf.extract();
                    else if (prod_q.front().id < cons_q.front().id)
                        buf.insert(value);
                    else
                        value = buf.extract();
                }
            }
        }
    } else if (rank < prod_num + 1) {  // producer process
        mt19937 gen {random_device {}()};
        uniform_int_distribution<int> distr(0, 255);

        // produce some values
        for (int i = 0; i < prod_count; ++i) {
            value = distr(gen);
            MPI_Ssend(&value, 1, MPI_INT, 0, PRODUCER, MPI_COMM_WORLD);
            MPI_Recv(&response, 1, MPI_INT, 0, PRODUCER, MPI_COMM_WORLD,
                    MPI_STATUSES_IGNORE);
            // std::this_thread::sleep_for(std::chrono::milliseconds(distr(gen)));
        }
        MPI_Ssend(&value, 1, MPI_INT, 0, FINISH, MPI_COMM_WORLD);

    } else if (rank < prod_num + cons_num + 1) {  // consumer process
        mt19937 gen {random_device {}()};
        uniform_int_distribution<int> distr(0, 255);

        // consume some values
        for (int i = 0; i < cons_count; ++i) {
            MPI_Ssend(&response, 1, MPI_INT, 0, CONSUMER, MPI_COMM_WORLD);
            MPI_Recv(&value, 1, MPI_INT, 0, CONSUMER, MPI_COMM_WORLD,
                    MPI_STATUSES_IGNORE);
            // std::this_thread::sleep_for(std::chrono::milliseconds(distr(gen)));
        }
        MPI_Ssend(&value, 1, MPI_INT, 0, FINISH, MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}
