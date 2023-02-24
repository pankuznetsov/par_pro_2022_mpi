// Copyright 2022 Kochetov Maksim
#ifndef MODULES_TASK_3_KOCHETOV_M_LIN_STRETCH_LIN_STRETCH_H_
#define MODULES_TASK_3_KOCHETOV_M_LIN_STRETCH_LIN_STRETCH_H_

#include <vector>

using ColorChannel = unsigned char;
using Bitmap = std::vector<ColorChannel>;


Bitmap genBitmap(int size, int minTon, int maxTon);

ColorChannel stretchTon(ColorChannel ton, ColorChannel minTon, ColorChannel maxTon);

void stretchBitmap(Bitmap* sourcePtr, ColorChannel minTon, ColorChannel maxTon);


Bitmap getLinearStretchingSequential(const Bitmap& source);

Bitmap getLinearStretchingParralel(const Bitmap& source);

#endif  // MODULES_TASK_3_KOCHETOV_M_LIN_STRETCH_LIN_STRETCH_H_
