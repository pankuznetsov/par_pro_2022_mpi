// Copyright 2022 Zagrebin S.

#include <mpi.h>
#include <cstring>
#include <cmath>
#include "../../../modules/task_1/zagrebin_s_sentence_count/sentence_count.h"

void countSentences(int *res, const char *str) {
    int rank, locRes = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
    {   int size;
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        *res = 0;
        int len = strlen(str) + 1;
        int b = ceil(static_cast<double>(len-1)/size + 1);
        int B = b;

        for (int i = 1; i < size; ++i)
        {   int start = i * (b - 1);
            if (start < len - 1) {
                if (start + b > len) b = len - start;
                MPI_Send(&b, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
                MPI_Send(str+start, b, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            } else {
                int zero = 0;
                MPI_Send(&zero, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }

        for (int i = 0; i < B - 1; ++i)
            if (str[i] == '.' && (str[i+1] == ' ' || str[i+1] == '\0'))
                ++locRes;
    } else {
        char *buff; int buffLen;
        MPI_Status status;
        MPI_Recv(&buffLen, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if (buffLen != 0)
        {   buff = new char[buffLen];
            MPI_Recv(buff, buffLen, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            for (int i = 0; i < buffLen-1; ++i)
                if (buff[i] == '.' && (buff[i+1] == ' ' || buff[i+1] == '\0'))
                    ++locRes;
            delete[] buff;
        }
    }

    MPI_Reduce(&locRes, res, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
}
