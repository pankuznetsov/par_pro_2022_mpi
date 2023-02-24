// Copyright 2023 Shaposhnikova Ekaterina
#ifndef MODULES_TASK_3_SHAPOSHNIKOVA_E_ALG_JARVIS_H_
#define MODULES_TASK_3_SHAPOSHNIKOVA_E_ALG_JARVIS_H_

#include <functional>
#include <vector>

std::vector<int> algJarvisSequential(std::vector<int> hull);
std::vector<int> algJarvisParallel(std::vector<int> hull, int proc_count);

#endif  // MODULES_TASK_3_SHAPOSHNIKOVA_E_ALG_JARVIS_H_
