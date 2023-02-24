#include<vector>
bool isHyperCube(int ProcNum);
void SendData(std::vector<int>& data,int n,int sender,int receiver,std::vector<int>& path);
void GenerateData(std::vector<int>& data,int n);
int GetNextNode(int current, int destination);