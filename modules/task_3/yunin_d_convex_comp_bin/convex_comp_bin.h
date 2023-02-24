// Copyright 2023 Yunin Daniil
#pragma once
#include <vector>
#include <string>

using std::vector;

vector<int> Labeling(const vector<vector<int>> &image, int width, int height);
void PrintImage(const vector<vector<int>> &image, int width, int height);
void PrintImage(const vector<int> &image, int width, int height);
void CreateLabelingImage(int image[], int width, int height);
bool Fill(const vector<vector<int>> &image, vector<vector<int>> *label_image,
    int x, int y, int label, int height, int width);
vector<int> CountComponents(const vector<int> &image, int width, int height);
vector<int> PointsComponents(const vector<int>& components_num_points, const vector<int>& image, int width);
int FindNumComponents(const vector<int> &image);
vector<int> MakeMinPointsLocal(const vector<int> &image, int width, int height, int component);
vector<int> GrahamAlgo(vector<int> points);
vector<int> MainFuncSequence(const vector<int> &image, int width, int height, int num_components);
vector<int> MainFunParallel(const vector<int> &image, int width, int height, int num_components);
int CountNumPointsComponent(vector<int> image);
void CreateComponent1(vector<vector<int>> *matr);
void CreateComponent2(vector<vector<int>> *matr);
void CreateComponent3(vector<vector<int>> *matr, int width, int height);
void CreateComponent4(vector<vector<int>> *matr, int width, int height);
void CreateComponent5(vector<vector<int>> *matr, int width, int height);
