// Copyright 2022 Zagrebin S.

#ifndef MODULES_TASK_3_ZAGREBIN_S_DIJKSTRA_DIJKSTRA_H_
#define MODULES_TASK_3_ZAGREBIN_S_DIJKSTRA_DIJKSTRA_H_

class Random {
 private:
    int coef;
 public:
    explicit Random(double q);

    static void Rand_Init(int seed);

    inline bool toss() noexcept;

    static int give(unsigned i) noexcept;
};

void GenSymMatr(int** m, int n, double p, int max_w);

struct V { int d, ind; };

V findMin(int* arr, int n);

void Dijkstra(int* m, int n, int source, int* res);


#endif  // MODULES_TASK_3_ZAGREBIN_S_DIJKSTRA_DIJKSTRA_H_
