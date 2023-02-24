// Copyright 2022 Nosonova Darina
#include "../../../modules/task_1/nosonova_d_string_sentences/string_sentences.h"

char *GetRandomString(int size, int pointChancePercent) {
    std::random_device dev;
    std::mt19937 gen(dev());

    char *str = new char[size];

    for (int i = 0; i < size; i++) {
        char symbol = static_cast<char>('a' + gen() % ('z' - 'a'));
        int percent = gen() % 101;
        if (pointChancePercent >= percent)
            symbol = '.';
        str[i] = symbol;
    }
    return str;
}

int GetSentencesQuantitySequence(char *str, int size, double *t) {
    int res = 0;
    double start = 0, end = 0;
    start = clock();
    for (int i = 0; i < size; i++) {
        if (str[i] == '.')
            res++;
    }
    end = clock();
    *t = (end - start) / 1000;
    return res;
}

int GetSentencesQuantityParallel(char *str, int size, double *t) {
    double start_time = 0, end_time = 0;
    start_time = MPI_Wtime();

    int proc_count;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int local_result = 0;

    if (rank == 0) {  // main process
        int current_index = 0;
        int size_cut = size / proc_count;
        int size_tail = size % proc_count;

        for (int i = 1; i < proc_count; i++) {
            char *str_cut = new char[size_cut];

            for (int j = 0; j < size_cut; j++, current_index++)
                str_cut[j] = str[current_index];

            MPI_Send(&size_cut, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(str_cut, size_cut, MPI_CHAR, i, 1, MPI_COMM_WORLD);
        }

        // main process: handle part + tail
        for (; current_index < size; current_index++)
            if (str[current_index] == '.')
                local_result++;

    } else {  // other process
        int local_size = 0;
        MPI_Recv(&local_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);
        char *local_str = new char[local_size];
        MPI_Recv(local_str, local_size, MPI_CHAR, 0, 1, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);

        for (int i = 0; i < local_size; i++)
            if (local_str[i] == '.')
                local_result++;
    }

    int answer = 0;
    MPI_Reduce(&local_result, &answer, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();
    *t = end_time - start_time;

    return answer;
}
