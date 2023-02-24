// Copyright 2022 Chernyshev Nikita
#include "../../../modules/task_1/chernyshev_n_rect_integr/rect_integr.h"

double *FuncValues_Sin(double from, double to, int val_quantity,
                       double *step_size) {
    double *func_values = new double[val_quantity];
    double step = (to - from) / val_quantity;
    double x_current = from + step / 2;
    func_values[0] = sin(x_current);

    for (int i = 1; i < val_quantity; i++) {
        x_current += step;
        func_values[i] = sin(x_current);
    }

    *step_size = step;
    return func_values;
}

double *FuncValues_X2(double from, double to, int val_quantity,
                      double *step_size) {
    double *func_values = new double[val_quantity];
    double step = (to - from) / val_quantity;
    double x_current = from + step / 2;
    func_values[0] = pow(x_current, 2);

    for (int i = 1; i < val_quantity; i++) {
        x_current += step;
        func_values[i] = pow(x_current, 2);
    }

    *step_size = step;
    return func_values;
}

double TrueIntegral_Sin(double from, double to) { return -cos(to) + cos(from); }

double TrueIntegral_X2(double from, double to) {
    return pow(to, 3) / 3 - pow(from, 3) / 3;
}

double IntegralParallel(double *func_values, double step_size,
                        int val_quantity) {
    int rank, proc_quant;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_quant);
    double local_result = 0;
    double global_result = 0;

    if (rank == 0) {
        int val_quant_for_proc = val_quantity / proc_quant;
        int val_quant_for_main_proc =
            val_quant_for_proc + val_quantity % proc_quant;

        for (int i = 1; i < proc_quant; i++) {
            int data_size = val_quant_for_proc;
            double step = step_size;
            double *data = new double[val_quant_for_proc];
            for (int j = 0; j < val_quant_for_proc; j++)
                data[j] = func_values[val_quant_for_main_proc +
                                      (i - 1) * val_quant_for_proc + j];

            MPI_Send(&data_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&step, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            MPI_Send(data, val_quant_for_proc, MPI_DOUBLE, i, 0,
                     MPI_COMM_WORLD);
        }

        for (int i = 0; i < val_quant_for_main_proc; i++)
            local_result += func_values[i] * step_size;

    } else {
        int size;
        double step;
        MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
        MPI_Recv(&step, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);
        double *values = new double[size];
        MPI_Recv(values, size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD,
                 MPI_STATUSES_IGNORE);
        for (int i = 0; i < size; i++)
            local_result += values[i] * step;
    }

    MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0,
               MPI_COMM_WORLD);
    return global_result;
}
