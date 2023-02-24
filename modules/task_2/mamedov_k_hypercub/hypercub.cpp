#include<vector>
#include<mpi.h>
#include"hypercub.h"
#include<iostream>
#include<random>
extern int ProcRank;
extern int ProcNum;
bool isHyperCube(int ProcNum)
{
	if (ProcNum <= 1)return false;
	while (ProcNum != 1)
	{
		if (ProcNum % 2 == 1)return false;
		ProcNum /= 2;
	}
	return true;
}
void GenerateData(std::vector<int>& data, int n)
{
	std::random_device dev;
	std::mt19937 rand_r(dev());
	for (int i = 0; i < n; i++)
	{
		data[i] = rand_r() % 1000;
	}
}
int GetNextNode(int current, int destination)
{
	//std::cout << "Current:" << current << " Destination:" << destination << std::endl;
	if (current == destination) {
		return destination;
	}
	int mask = 1;
	while (true) {
		if ((current & mask) == (destination & mask)) {
			mask = mask << 1;
			continue;
		}

		if ((current & mask) == 0) {
			//std::cout << "Returning " << current + mask << std::endl;
			return current + mask;
		}
		else {
			//std::cout << "Returning " << current - mask << std::endl;
			return current - mask;
		}
	}
}
void SendData(std::vector<int>& data, int n, int sender, int receiver, std::vector<int>& path)
{
	path.push_back(sender);
	if (sender == receiver)return;
	int next = GetNextNode(sender, receiver);
	if (ProcRank == sender)
	{
		//std::cout << "Sending From " << ProcRank << std::endl;
		MPI_Send(data.data(), n, MPI_INT, next, 0, MPI_COMM_WORLD);
	}
	else if (ProcRank == next)
	{
		MPI_Status status;
		//std::cout << "Receiving From " << ProcRank <<" Sender is "<<sender<< std::endl;
		MPI_Recv(data.data(), n, MPI_INT, sender, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	SendData(data, n, next, receiver, path);
}