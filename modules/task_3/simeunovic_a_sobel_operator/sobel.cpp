#include<vector>
#include"sobel.h"
#include<mpi.h>
#include<random>
#include<iostream>
extern int ProcRank;
extern int ProcSize;
void CreateRandomImage(std::vector<int>& image, int h, int w)
{
	std::random_device dev;
	std::mt19937 gen(dev());
	image.resize(h * w);
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			image[i * w + j] = gen() % 256;
		}
	}
}
int Clamp(int min, int value, int max)
{
	if (value < min)return min;
	if (value > max)return max;
	return value;
}
void CalculateSober(std::vector<int>& image, int h, int w, std::vector<int>& resultX, std::vector<int>& resultY)
{
	image.resize(h * w);
	MPI_Bcast(image.data(), h * w, MPI_INT, 0, MPI_COMM_WORLD);
	//std::cout << "Started From " << ProcRank << std::endl;
	int RestRows = h-2;
	int RowsFromStart = 0;
	for (int i = 0; i < ProcRank; i++)
	{
		int taking= RestRows / (ProcSize - i);
		RestRows = RestRows-taking;
		RowsFromStart += taking;
	}
	int RowNum = RestRows / (ProcSize - ProcRank);
	int StartRow = RowsFromStart;
	std::vector<int>pProcResultX(RowNum * (w - 2));
	std::vector<int>pProcResultY(RowNum * (w - 2));
	ParallelCalculation(image, h, w, StartRow, RowNum, pProcResultX, pProcResultY);
	if (ProcRank == 0)
	{
		resultX.resize((h - 2) * (w - 2));
		resultY.resize((h - 2) * (w - 2));
	}
	DataReceiving(pProcResultX, pProcResultY, h, w, resultX, resultY);
}
void ParallelCalculation(std::vector<int>& image, int h, int w, int StartRow, int RowNum, std::vector<int>& pProcResultX, std::vector<int>& pProcResultY)
{
	std::vector<int>filterX = { -1,0,1,-2,0,2,-1,0,1 };
	std::vector<int>filterY = { 1,2,1,0,0,0,-1,-2,-1 };
	for (int i = StartRow; i < StartRow + RowNum; i++)
	{
		for (int j = 0; j < w - 2; j++)
		{
			int sumX = 0;
			int sumY = 0;
			for (int i1 = 0; i1 < 3; i1++)
			{
				for (int j1 = 0; j1 < 3; j1++)
				{
					sumX += image[(i + i1) * w + (j + j1)] * (filterX[i1 * 3 + j1]);
					sumY+= image[(i + i1) * w + (j + j1)] * (filterY[i1 * 3 + j1]);
				}
			}
			pProcResultX[(i - StartRow) * (w-2) + j] = Clamp(0,sumX,255);
			pProcResultY[(i - StartRow) * (w-2) + j] = Clamp(0,sumY,255);
		}
	}
}
void DataReceiving(std::vector<int>& pProcResultX, std::vector<int>& pProcResultY, int h, int w, std::vector<int>& resultX, std::vector<int>& resultY)
{
	std::vector<int>pRecvNum(ProcSize);
	std::vector<int>pRecvInd(ProcSize);
	int RestRows = h - 2;
	int RowNum = RestRows / ProcSize;
	pRecvNum[0] = RowNum * (w - 2);
	pRecvInd[0] = 0;
	for (int i = 1; i < ProcSize; i++)
	{
		RestRows -= RowNum;
		RowNum = RestRows / (ProcSize - i);
		pRecvNum[i] = RowNum * (w - 2);
		pRecvInd[i] = pRecvInd[i - 1] + pRecvNum[i - 1];
	}
	MPI_Gatherv(pProcResultX.data(), pRecvNum[ProcRank], MPI_INT, resultX.data(), pRecvNum.data(), pRecvInd.data(), MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Gatherv(pProcResultY.data(), pRecvNum[ProcRank], MPI_INT, resultY.data(), pRecvNum.data(), pRecvInd.data(), MPI_INT, 0, MPI_COMM_WORLD);
}
void SequentalCalculation(std::vector<int>& image, int h, int w, std::vector<int>& SeqResultX,std::vector<int>& SeqResultY)
{
	SeqResultX.resize((h - 2) * (w - 2));
	SeqResultY.resize((h - 2) * (w - 2));
	std::vector<int>filterX = { -1,0,1,-2,0,2,-1,0,1 };
	std::vector<int>filterY = { 1,2,1,0,0,0,-1,-2,-1 };
	for (int i = 0; i < h - 2; i++)
	{
		for (int j = 0; j < w - 2; j++)
		{
			int sumX = 0;
			int sumY = 0;
			for (int i1 = 0; i1 < 3; i1++)
			{
				for (int j1 = 0; j1 < 3; j1++)
				{
					sumX += image[(i + i1) * w + (j + j1)] * (filterX[i1 * 3 + j1]);
					sumY += image[(i + i1) * w + (j + j1)] * (filterY[i1 * 3 + j1]);
				}
			}
			SeqResultX[i * (w - 2) + j] = Clamp(0,sumX,255);
			SeqResultY[i * (w - 2) + j] = Clamp(0,sumY,255);
		}
	}
}
/*void DataDistribution(std::vector<int>& image, int h, int w, send2int& receiver)
{
	std::vector<send2int>SendBuf(ProcSize);
	int RestRows = h-2;
	int RowNum = RestRows / ProcSize;
	SendBuf[0].send_num = RowNum;
	SendBuf[0].start_ind = 0;
	for (int i = 1; i < ProcSize; i++)
	{
		RestRows -= RowNum;
		RowNum = RestRows / (ProcSize - i);
		SendBuf[i].send_num = RowNum;
		SendBuf[i].start_ind = SendBuf[i - 1].start_ind + SendBuf[i - 1].send_num;
	}
	MPI_Scatter(SendBuf.data(),1,MPI_2INT,&receiver,1,MPI_2INT,)
}*/
