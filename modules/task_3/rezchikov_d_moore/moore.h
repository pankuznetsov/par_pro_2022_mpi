#include <vector>
#include <mpi.h>
#include <random>
#include <iostream>
#include <iomanip>

std::vector<int> getRandomGrapgh(int size);
void MooreParallel(std::vector<int> & graph, int srcVer, std::vector<int> & res, bool * cycle_flag);
std::vector<int> MooreSequential(std::vector<int> & graph, int srcVer, bool * cycle_flag);