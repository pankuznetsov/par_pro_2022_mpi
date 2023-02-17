// Copyright 2023 Shaposhnikova Ekaterina
#include "../../../modules/task_3/shaposhnikova_e_alg_jarvis/alg_jarvis.h"

#include <mpi.h>
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>


std::vector<int> algJarvisSequential(std::vector<int> hull) {
    std::vector<int> resArr;

    int start_id = 100;
    int start_x = 100;
    int start_y = 100;

    for (int i = 0; i < hull.size(); i += 2) {
      if (hull[i] < start_x) {
        start_id = i;
        start_x = hull[i];
        start_y = hull[i + 1];
      }
      else if (hull[i] == start_x && hull[i + 1] < start_y) {
        start_id = i;
        start_x = hull[i];
        start_y = hull[i + 1];
      }     
    }

    int f_x = start_x;
    int f_y = start_y;

    std::swap(hull[start_id], hull[hull.size() - 2]);
    std::swap(hull[start_id + 1], hull[hull.size() - 1]);
    hull.pop_back(), hull.pop_back();

    resArr.push_back(f_x);
    resArr.push_back(f_y);

    int point_pos = 0;

    for (int i = 0; i < hull.size(); i += 2) {
      int Ax = hull[i] - start_x;
      int Bx = hull[point_pos] - start_x;

      int Ay = hull[i + 1] - start_y;
      int By = hull[point_pos + 1] - start_y;

      if ((Ax * By) - (Ay * Bx) > 0) {
        point_pos = i;
      }
    }
    int x = hull[point_pos];
    int y = hull[point_pos + 1];

    hull.push_back(f_x);
    hull.push_back(f_y);

    while (!(x == f_x && y == f_y)) {
        resArr.push_back(x);
        resArr.push_back(y);

        std::swap(hull[point_pos], hull[hull.size() - 2]);
        std::swap(hull[point_pos + 1], hull[hull.size() - 1]);
        hull.pop_back(), hull.pop_back();

        start_x = x, start_y = y;
        point_pos = 0;

        for (int i = 0; i < hull.size(); i += 2) {
          int Ax = hull[i] - start_x;
          int Bx = hull[point_pos] - start_x;

          int Ay = hull[i + 1] - start_y;
          int By = hull[point_pos + 1] - start_y;

          if ((Ax * By) - (Ay * Bx) > 0) {
            point_pos = i;
          }
        }
        x = hull[point_pos];
        y = hull[point_pos + 1];
    }

    return resArr;
}

std::vector<int> algJarvisParallel(std::vector<int> hull, int proc_count) {
    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int procCounter = std::min(comm_size, proc_count / 4);
    if (rank >= procCounter) return std::vector<int>(0);

    const int m= proc_count / procCounter;
    const int r = proc_count % procCounter;

    std::vector<int> local_res;
    std::vector<int> local_points(2 * m);

    if (rank == 0) {
        local_points =
            std::vector<int>(hull.begin(), hull.begin() + 2 * (m+ r));

        for (int i = 1; i < procCounter; i++) {
          MPI_Send(hull.data() + 2 * (m + r) + 2 * m * (i - 1), 2 * m, MPI_INT,
            i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status status;
        MPI_Recv(local_points.data(), 2 * m, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 &status);
    }

    local_res = algJarvisSequential(local_points);

    if (rank != 0) {
        MPI_Send(local_res.data(), static_cast<int>(local_res.size()), MPI_INT, 0, 0,
                 MPI_COMM_WORLD);
    } else {
        std::vector<int> points_processed(2 * proc_count);
        int tot_elem = 0;
        for (int i = 0; i < static_cast<int>(local_res.size()); i++)
            points_processed[i] = local_res[i];
        tot_elem += local_res.size();

        for (int i = 1; i < procCounter; i++) { 
            MPI_Status status;
            MPI_Recv(points_processed.data() + tot_elem, 2 * m, MPI_INT,
                     i, 0, MPI_COMM_WORLD, &status);
            int cur_received_count = 0;
            MPI_Get_count(&status, MPI_INT, &cur_received_count);

            tot_elem += cur_received_count;
        }

        points_processed.resize(tot_elem);

        return algJarvisSequential(points_processed);
    }

    return std::vector<int>(0);
}
