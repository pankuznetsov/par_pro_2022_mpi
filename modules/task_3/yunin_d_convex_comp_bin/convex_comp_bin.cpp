// Copyright 2023 Yunin Daniil
#include <mpi.h>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <random>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_3/yunin_d_convex_comp_bin/convex_comp_bin.h"

using std::vector;

// 1 - Labeling
vector<int> Labeling(const vector<vector<int>> &image, int width, int height) {
    int label = 1;
    vector<vector<int>> labeling_image_arr(height, vector<int>(width));
    // start init array
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            labeling_image_arr[i][j] = 0;
        }
    }
    // end init array
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (Fill(image, &labeling_image_arr, x, y, label, height, width)) {
                label++;
            }
            // std::cout << "\nNext point\n\n";
        }
    }
    // start print array
    // for (int i = 0; i < height; i++) {
    //     for (int j = 0; j < width; j++) {
    //         std::cout << labeling_image_arr[i][j] << ' ';
    //     }
    //     std::cout << std::endl;
    // }
    // end print array
    vector<int> labeling_image(width*height);
    // start init vector
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            labeling_image[i * width + j] = labeling_image_arr[i][j];
        }
    }
    // end init vector
    // start print vector
    // std::cout << "print vector";
    // for (int i = 0; i < height * width; i++) {
    //     if ((i % width == 0)) {
    //         std::cout << "\n" << labeling_image[i] << ' ';
    //         continue;
    //     }
    //     std::cout << labeling_image[i] << ' ';
    // }
    // end print vector
    return labeling_image;
}

bool Fill(const vector<vector<int>> &image, vector<vector<int>> *label_image,
    int x, int y, int label, int height, int width) {
    // std::cout << label << std::endl;
    bool next_step = false;
    if (((*label_image)[y][x] == 0) && (image[y][x] == 1)) {
        next_step = true;
        (*label_image)[y][x] = label;
        if (x > 0) {
            // std::cout << "Left\n";
            Fill(image, label_image, x - 1, y, label, height, width);
        }
        if (x < width - 1) {
            // std::cout << "Right\n";
            Fill(image, label_image, x + 1, y, label, height, width);
        }
        if (y > 0) {
            // std::cout << "Down\n";
            Fill(image, label_image, x, y - 1, label, height, width);
        }
        if (y < height - 1) {
            // std::cout << "Up\n";
            Fill(image, label_image, x, y + 1, label, height, width);
        }
    }
    return next_step;
}

void PrintImage(const vector<vector<int>> &image, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << image[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void PrintImage(const vector<int> &image, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << image[i * width + j] << " ";
        }
        std::cout << std::endl;
    }
}

void CreateLabelingImage(int image[], int width, int height) {
    int default_value = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; i++) {
            image[i * width + j] = 0;
        }
    }
}

vector<int> CountComponents(const vector<int> &image, int width, int height) {
    int num_components = 0;
    for (int i = 0; i < width * height; i++) {
        if (image[i] > num_components) {
            num_components = image[i];
        }
    }
    vector<int> num_points_component(num_components);
    vector<int> components(num_components);
    for (int i = 1; i <= num_components; i++) {
        components[i-1] = i;
    }
    for (int i = 0; i < width * height; i++) {
        for (int j = 0; j < num_components; j++) {
            if (components[j] == image[i]) {
                num_points_component[j]++;
                break;
            }
        }
    }
    vector<int> result(num_components * 2);
    int j = 0;
    for (int i = 0; i < num_components * 2; i++) {
        if (i % 2 == 0) {
            result[i] = components[j];
        } else {
            result[i] = num_points_component[j];
            j++;
        }
    }
    // // start print
    // std::cout << "\n";
    // for (int i = 0; i < num_components * 2; i+=2) {
    //     std::cout << result[i] << " " << result[i + 1] << std::endl;
    // }
    // end print
    return result;
}

int FindNumComponents(const vector<int> &image) {
    int num_components = 0;
    for (int i = 0; i < image.size(); i++) {
        if (image[i] > num_components) {
            num_components = image[i];
        }
    }
    return num_components;
}

vector<int> PointsComponents(const vector<int>& components_num_points, const vector<int>& image, int width) {
    int size = 0;
    for (int i = 1; i < components_num_points.size(); i+=2) {
        size += components_num_points[i];
    }
    size = size * 2;
    size += components_num_points[components_num_points.size() - 2];
    vector<int> points(size);
    std::map<int, vector<int>> map_points;
    for (int i = 0; i < image.size(); i++) {
        for (int j = 0; j < components_num_points.size(); j+=2) {
            if (components_num_points[j] == image[i]) {
                map_points[image[i]].push_back(i);
            }
        }
    }
    // start print
    // for (int i = 1; i <= components_num_points[components_num_points.size() - 2]; i++) {
    //     std::cout << "point " << i << std::endl;
    //     for (int j = 0; j < map_points[i].size(); j++) {
    //         std::cout << map_points[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // end print
    int k = 0;
    for (int i = 1; i <= components_num_points[components_num_points.size() - 2]; i++) {
        for (int j = 0; j < map_points[i].size(); j++) {
            points[k] = map_points[i][j] % width;
            k++;
            points[k] = map_points[i][j] / width;
            k++;
        }
        points[k] = -1;
        k++;
    }
    // start print
    // for (int i = 0; i < size; i++) {
    //     std::cout << points[i] << " ";
    // }
    // std::cout << std::endl;
    // end print
    return points;
}
// 1 - Labeling

// 2 - start make min points
int CountNumPointsComponent(vector<int> image) {
    int num_points = 0;
    for (int i = 0; i < image.size(); i++) {
        if (image[i] != 0) {
            num_points++;
        }
    }
    return num_points;
}

vector<int> MakeMinPointsLocal(const vector<int> &image, int width, int height, int component) {
    vector<int> local_image(image);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image[i * width + j] == component) {
                if ((j > 0) && (j < width - 1)) {
                    if ((i == 0) || (i == height - 1)) {
                        if ((image[i * width + j - 1] == component) && (image[i * width + j + 1] == component)) {
                            local_image[i * width + j] = 0;
                        }
                    }
                    if ((i > 0) && (i < height - 1)) {
                        if (((image[i * width + j - 1] == component) && (image[i * width + j + 1] == component)) ||
                            ((image[(i + 1) * width + j] == component) && (image[(i - 1) * width + j] == component))) {
                            local_image[i * width + j] = 0;
                        }
                    }
                    continue;
                }
                if ((i > 0) && (i < height - 1)) {
                    if ((j == 0) || (j == width - 1)) {
                        if ((image[(i - 1) * width + j] == component) &&
                            (image[(i + 1) * width + j] == component)) {
                            local_image[i * width + j] = 0;
                        }
                    }
                    if ((j > 0) && (j < width - 1)) {
                        if (((image[i * width + j - 1] == component) && (image[i * width + j + 1] == component)) ||
                            ((image[(i + 1) * width + j] == component) && (image[(i - 1) * width + j] == component))) {
                            local_image[i * width + j] = 0;
                        }
                    }
                }
            } else {
                local_image[i * width + j] = 0;
            }
        }
    }
    // std::cout << "\nPrint for " << component << "\n\n";
    // PrintImage(local_image, width, height);
    int size = CountNumPointsComponent(local_image);
    vector<int> points(size * 2);
    int k = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (local_image[i * width + j] != 0) {
                points[k] = j;  // write x coord
                k++;
                points[k] = i;  // write y coord
                k++;
            }
        }
    }
    // print
    // for (int i = 0; i < points.size(); i+=2) {
    //     std::cout << "(" << points[i] << "," << points[i + 1] << ")" << " ";
    // }
    // std::cout << std::endl;
    // end print
    return points;
}
// 2 - end make min points

// 3 start graham algorithm
int rotate(int x1, int y1, int x2, int y2, int x3, int y3) {
    return ((x2 - x1) * (y3 - y2) - (x3 - x2) * (y2 - y1));
}


vector<int> SortingPoints(const vector<int> &points, int x_min, int y_min) {
    vector<int> result(points);
    // print
    // for (int i = 0; i < result.size(); i+=2) {
    //     std::cout << "(" << result[i] << "," << result[i + 1] << ")" << " ";
    // }
    // std::cout << std::endl;
    // end print
    int size = points.size() / 2;
    for (int i = 1; i < size; i++) {
        int j = i;
        while ((j > 0) && (rotate(x_min, y_min, result[2 * j - 2],
            result[2 * j - 1], result[2 * j], result[2 * j + 1]) < 0)) {
            int temp = result[2 * j - 2];
            result[2 * j - 2] = result[2 * j];
            result[2 * j] = temp;
            temp = result[2 * j - 1];
            result[2 * j - 1] = result[2 * j + 1];
            result[2 * j + 1] = temp;
            j--;
        }
    }
    // print
    // for (int i = 0; i < result.size(); i+=2) {
    //     std::cout << "(" << result[i] << "," << result[i + 1] << ")" << " ";
    // }
    // std::cout << std::endl;
    // end print
    return result;
}

vector<int> GrahamAlgo(vector<int> points) {
    vector<int> result;
    int num_points = points.size() / 2;
    if (num_points > 1) {
        int x_min = points[0];
        int y_min = points[1];
        int min_index = 0;
        for (int i = 2; i < points.size(); i+=2) {
            if (points[i] < x_min || (points[i] == x_min && points[i + 1] < y_min)) {
                x_min = points[i];
                y_min = points[i + 1];
                min_index = i;
            }
        }
        // delete first point from vector
        int temp = points[min_index];
        points[min_index] = points[num_points * 2 - 2];
        points[num_points * 2 - 2] = temp;
        temp = points[min_index + 1];
        points[min_index + 1] = points[num_points * 2 - 1];
        points[num_points * 2 - 1] = temp;
        points.pop_back();
        points.pop_back();
        // std::cout << "\nxMin = " << x_min << ", yMin = " << y_min << '\n';
        points = SortingPoints(points, x_min, y_min);
        // start print
        // for (int i = 0; i < points.size(); i+=2) {
        //     std::cout << "(" << points[i] << "," << points[i + 1] << ")" << " ";
        // }
        // std::cout << std::endl;
        // end print
        result.push_back(x_min);
        result.push_back(y_min);
        result.push_back(points[0]);
        result.push_back(points[1]);
        for (int i = 2; i < points.size(); i += 2) {
            int result_size = result.size();
            // p1
            int x1 = result[result_size - 4];
            int y1 = result[result_size - 3];
            // p2
            int x2 = result[result_size - 2];
            int y2 = result[result_size - 1];
            // p3
            int x3 = points[i];
            int y3 = points[i + 1];

            int rot = rotate(x1, y1, x2, y2, x3, y3);
            // p1, p2, p3 on one line -> replacing p2 with p3
            if (rot == 0) {
                result[result_size - 2] = x3;
                result[result_size - 1] = y3;
            } else if (rot < 0) {
                // While p1, p2, p3 form right turn -> excluding p2
                while (rotate(result[(result.size()) - 4], result[(result.size()) - 3],
                            result[(result.size()) - 2], result[(result.size()) - 1], x3, y3) < 0)
                    result.pop_back(), result.pop_back();
                result.push_back(x3);
                result.push_back(y3);
            } else {
                // left turns are ok - just adding p3 to our result
                result.push_back(x3);
                result.push_back(y3);
            }
        }
        // start print
        // for (int i = 0; i < result.size(); i+=2) {
        //     std::cout << "(" << result[i] << "," << result[i + 1] << ")" << " ";
        // }
        // std::cout << std::endl;
        // end print
    } else {
        result.resize(2);
        result[0] = points[0];
        result[1] = points[1];
    }
    return result;
}
// 3 end graham algorithm

// start 4 components
void CreateComponent1(vector<vector<int>> *matr) {
    (*matr)[0][0] = 1; (*matr)[2][3] = 1; (*matr)[1][3] = 1;
    (*matr)[2][0] = 1; (*matr)[0][2] = 1; (*matr)[2][2] = 1;
    (*matr)[1][1] = 1; (*matr)[0][1] = 1; (*matr)[2][1] = 1;
    (*matr)[9][9] = 1; (*matr)[9][8] = 1; (*matr)[9][7] = 1;
    (*matr)[9][6] = 1; (*matr)[9][5] = 1; (*matr)[9][4] = 1;
    (*matr)[9][1] = 1; (*matr)[4][4] = 1; (*matr)[4][5] = 1;
    (*matr)[4][3] = 1; (*matr)[4][6] = 1; (*matr)[3][6] = 1;
    (*matr)[8][2] = 1; (*matr)[7][8] = 1; (*matr)[6][2] = 1;
}

void CreateComponent2(vector<vector<int>> *matr) {
    (*matr)[1][1] = 1;
    (*matr)[2][1] = 1;
    (*matr)[3][1] = 1;
    (*matr)[2][2] = 1;
    (*matr)[0][1] = 1;
}

void CreateComponent3(vector<vector<int>> *matr, int width, int height) {
    (*matr)[1][1] = 1; (*matr)[2][1] = 1; (*matr)[3][1] = 1;
    (*matr)[1][2] = 1; (*matr)[2][2] = 1; (*matr)[3][2] = 1;
    (*matr)[1][3] = 1; (*matr)[2][3] = 1; (*matr)[3][3] = 1;
    (*matr)[1][11] = 1; (*matr)[2][11] = 1; (*matr)[3][11] = 1;
    (*matr)[1][12] = 1; (*matr)[2][12] = 1; (*matr)[3][12] = 1;
    (*matr)[1][13] = 1; (*matr)[2][13] = 1; (*matr)[3][13] = 1;
    (*matr)[11][1] = 1; (*matr)[12][1] = 1; (*matr)[13][1] = 1;
    (*matr)[11][2] = 1; (*matr)[12][2] = 1; (*matr)[13][2] = 1;
    (*matr)[11][3] = 1; (*matr)[12][3] = 1; (*matr)[13][3] = 1;
    (*matr)[11][11] = 1; (*matr)[12][11] = 1; (*matr)[13][11] = 1;
    (*matr)[11][12] = 1; (*matr)[12][12] = 1; (*matr)[13][12] = 1;
    (*matr)[11][13] = 1; (*matr)[12][13] = 1; (*matr)[13][13] = 1;
}



void CreateComponent4(vector<vector<int>> *matr, int width, int height) {
    for (int i = 0; i < height; i++) {
        if (i % 2 == 0) {
           for (int j = 0; j < width; j++) {
               (*matr)[i][j] = 1;
            }
        } else {
            for (int j = 1; j < width; j+=2) {
                (*matr)[i][j] = 1;
            }
        }
    }
}

void CreateComponent5(vector<vector<int>> *matr, int width, int height) {
    for (int i = 0; i < height; i++) {
        if (i == 0 || i == height - 1) {
           for (int j = 0; j < width; j++) {
               (*matr)[i][j] = 1;
            }
        } else {
            (*matr)[i][0] = 1;
            (*matr)[i][width - 1] = 1;
        }
    }
    (*matr)[2][2] = 1; (*matr)[1][1] = 1; (*matr)[3][3] = 1;
    (*matr)[3][1] = 1; (*matr)[1][3] = 1;
}
// end 4 components

vector<int> MainFuncSequence(const vector<int> &image, int width, int height, int num_components) {
    vector<int> result;
    for (int i = 1; i <= num_components; i++) {
        vector<int> local_image(image);
        vector<int> points_component = MakeMinPointsLocal(local_image, width, height, i);
        vector<int> convex_shell = GrahamAlgo(points_component);
        // start print
        // std::cout << "convex shell" <<std::endl;
        // for (int i = 0; i < convex_shell.size(); i+=2) {
        //     std::cout << "(" << convex_shell[i] << "," << convex_shell[i + 1] << ")" << " ";
        // }
        // std::cout << std::endl;
        // end print
        for (int j = 0; j < convex_shell.size(); j++) {
            result.push_back(convex_shell[j]);
        }
        result.push_back(-1);
    }
    // start print
    // std::cout << "result" <<std::endl;
    // for (int i = 0; i < result.size() - 2; i+=2) {
    //     if (result[i] == -1) {
    //         std::cout << result[i] << " ";
    //         i++;
    //     }
    //     std::cout << "(" << result[i] << "," << result[i + 1] << ")" << " ";
    // }
    // std::cout << std::endl;
    // end print
    return result;
}


vector<int> MainFunParallel(const vector<int> &image, int width, int height, int num_components) {
    vector<int> result;
    int comm_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // std::cout << "all proc " << comm_size << " rank " << rank << "\n";
    const int num_active_proc = std::min(comm_size, num_components);
    // std::cout << "num active proc " << num_active_proc << "\n";
    // std::cout << "num compoments " << num_components << "\n";
    if (rank >= num_active_proc) {
        return vector<int>(0);
    }
    vector<int> components(num_components);
    for (int i = 0; i < num_components; i++) {
        components[i] = i+1;
    }
    int part = num_components / num_active_proc;
    int remainder = num_components % num_active_proc;
    int local_parts = part;
    if (rank < remainder) {
        local_parts++;
    }
    vector<int> local_components(local_parts);
    vector<int> local_image(width * height);
    if (rank == 0) {
        local_image = image;
        for (int i = 1; i < num_active_proc; i++) {
            // std::cout << "Send 1\n";
            MPI_Send(local_image.data(), width * height, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Status stat;
        // std::cout << "Recv 1\n";
        MPI_Recv(local_image.data(), width * height, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
    }
    if (rank != 0) {
        MPI_Send(&local_parts, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    if (rank == 0) {
        int index = 0;
        for (int i = 1; i < num_active_proc; i++) {
            // index = (i-1) * part;
            int proc_part;
            MPI_Status stat;
            MPI_Recv(&proc_part, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &stat);
            // std::cout << "rank = " << rank << " index = " << index << " proc_part " << proc_part << "\n";
            // std::cout << "Send 2\n";
            // std::cout << " rank = " << i << " part = " << proc_part << "\n";
            MPI_Send(components.data() + index, proc_part, MPI_INT, i, 0, MPI_COMM_WORLD);
            index += proc_part;
        }
        // index += part;
        // if (num_active_proc == 1) {
        //     index--;
        // }
        // std::cout << "rank = " << rank << " index = " << index << " part " << local_parts << "\n";
        local_components.resize(local_parts);
        int j = 0;
        for (int i = index; i < num_components; i++) {
            local_components[j] = components[i];
            j++;
        }
    } else {
        MPI_Status stat;
        // std::cout << "Recv 1\n";
        MPI_Recv(local_components.data(), local_parts, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
    }
    // std::cout << "Proc " << rank << "\n";
    // PrintImage(local_image, width, height);
    // for (int i = 0; i < local_components.size(); i++) {
    //     std::cout << local_components[i] << ' ';
    // }
    // std::cout << "\n";
    // Same operations for process
    vector<int> local_result;
    for (int i = 0; i < local_components.size(); i++) {
        vector<int> points_component = MakeMinPointsLocal(local_image, width, height, local_components[i]);
        vector<int> convex_shell = GrahamAlgo(points_component);
        for (int j = 0; j < convex_shell.size(); j++) {
            local_result.push_back(convex_shell[j]);
        }
        local_result.push_back(-1);
    }
    // print
    // std::cout << "Proc " << rank << "\n";
    // for (int i = 0; i < local_result.size(); i++) {
    //     std::cout << local_result[i] << ' ';
    // }
    // std::cout << "\n";
    // print
    if (rank != 0) {
        // std::cout << "Send 3\n";
        MPI_Send(local_result.data(), local_result.size(), MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        int size = 2 * CountNumPointsComponent(local_image);
        size += FindNumComponents(local_image);
        // std::cout << "recv 3\n";
        for (int i = 1; i < num_active_proc; i++) {
            vector<int> buffer(size);
            MPI_Status stat;
            MPI_Recv(buffer.data(), size, MPI_INT, i, 0, MPI_COMM_WORLD, &stat);
            // print
            // std::cout << "\nRank = " << i << " Size = " << size << std::endl;
            // for (int i = 0; i < size; i++) {
                // if (buffer[i] == -1 && buffer[i+1] == 0) {
                //     std::cout << buffer[i] << " ";
                //     break;
                // }
                // std::cout << buffer[i] << " ";
            // }
            // std::cout << "\n";
            // print
            for (int i = 0; i < size; i++) {
                if (buffer[i] == -1 && buffer[i+1] == 0) {
                    result.push_back(buffer[i]);
                    break;
                }
                result.push_back(buffer[i]);
            }
        }
        // print
        // for (int i = 0; i < local_result.size(); i++) {
        //     std::cout << local_result[i] << " ";
        // }
        // std::cout << "\n";
        // print
        for (int i = 0; i < local_result.size(); i++) {
            result.push_back(local_result[i]);
        }
    }
    return result;
}
