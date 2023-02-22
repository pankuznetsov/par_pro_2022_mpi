#include<gtest/gtest.h>
#include<mpi.h>
#include"gtest-mpi-listener.hpp"
#include<iostream>
#include<random>
#include<utility>
#include"sobel.h"
int ProcRank,ProcSize;
/*struct two_int
{
	int first;
	int second;
};*/
TEST(Sobel_Test, Test_1)
{
	MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	int h, w;
	std::pair<int, int> par;
	std::vector<int>image;
	std::vector<int>resultX;
	std::vector<int>resultY;
	//std::cout << "Started Main From " << ProcRank << std::endl;
	if (ProcRank == 0)
	{
		std::random_device dev;
		std::mt19937 gen(dev());
		h = gen() % 100 + ProcSize;
		w = gen() % 100 + ProcSize;
		CreateRandomImage(image, h, w);
		par.first = h;
		par.second = w;
	}
	//std::cout << "Exited" << std::endl;
	//std::cout << "Before Bcast" << std::endl;
	MPI_Bcast(&par, 1, MPI_2INT, 0, MPI_COMM_WORLD);
	//std::cout << "After Bcast" << std::endl;
	//std::cout << par[0] << " " << par[1] << std::endl;
	h = par.first;
	w = par.second;
	//std::cout << "Hello From " << ProcRank << std::endl;
	//MPI_Barrier(MPI_COMM_WORLD);
	CalculateSober(image, h, w, resultX, resultY);
	//MPI_Barrier(MPI_COMM_WORLD);
	if (ProcRank == 0)
	{
		std::vector<int>SeqResultX;
		std::vector<int>SeqResultY;
		SequentalCalculation(image, h, w, SeqResultX, SeqResultY);
		ASSERT_EQ(resultX, SeqResultX);
		ASSERT_EQ(resultY, SeqResultY);
	}
	MPI_Barrier(MPI_COMM_WORLD);
}
TEST(Sobel_Test, Test_2)
{
	MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	int h, w;
	std::pair<int, int> par;
	std::vector<int>image;
	std::vector<int>resultX;
	std::vector<int>resultY;
	//std::cout << "Started Main From " << ProcRank << std::endl;
	if (ProcRank == 0)
	{
		std::random_device dev;
		std::mt19937 gen(dev());
		h = gen() % 100 + ProcSize;
		w = gen() % 100 + ProcSize;
		CreateRandomImage(image, h, w);
		par.first = h;
		par.second = w;
	}
	//std::cout << "Exited" << std::endl;
	//std::cout << "Before Bcast" << std::endl;
	MPI_Bcast(&par, 1, MPI_2INT, 0, MPI_COMM_WORLD);
	//std::cout << "After Bcast" << std::endl;
	//std::cout << par[0] << " " << par[1] << std::endl;
	h = par.first;
	w = par.second;
	//std::cout << "Hello From " << ProcRank << std::endl;
	//MPI_Barrier(MPI_COMM_WORLD);
	CalculateSober(image, h, w, resultX, resultY);
	//MPI_Barrier(MPI_COMM_WORLD);
	if (ProcRank == 0)
	{
		std::vector<int>SeqResultX;
		std::vector<int>SeqResultY;
		SequentalCalculation(image, h, w, SeqResultX, SeqResultY);
		ASSERT_EQ(resultX, SeqResultX);
		ASSERT_EQ(resultY, SeqResultY);
	}
	MPI_Barrier(MPI_COMM_WORLD);
}
TEST(Sobel_Test, Test_3)
{
	MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	int h, w;
	std::pair<int, int> par;
	std::vector<int>image;
	std::vector<int>resultX;
	std::vector<int>resultY;
	//std::cout << "Started Main From " << ProcRank << std::endl;
	if (ProcRank == 0)
	{
		std::random_device dev;
		std::mt19937 gen(dev());
		h = gen() % 100 + ProcSize;
		w = gen() % 100 + ProcSize;
		CreateRandomImage(image, h, w);
		par.first = h;
		par.second = w;
	}
	//std::cout << "Exited" << std::endl;
	//std::cout << "Before Bcast" << std::endl;
	MPI_Bcast(&par, 1, MPI_2INT, 0, MPI_COMM_WORLD);
	//std::cout << "After Bcast" << std::endl;
	//std::cout << par[0] << " " << par[1] << std::endl;
	h = par.first;
	w = par.second;
	//std::cout << "Hello From " << ProcRank << std::endl;
	//MPI_Barrier(MPI_COMM_WORLD);
	CalculateSober(image, h, w, resultX, resultY);
	//MPI_Barrier(MPI_COMM_WORLD);
	if (ProcRank == 0)
	{
		std::vector<int>SeqResultX;
		std::vector<int>SeqResultY;
		SequentalCalculation(image, h, w, SeqResultX, SeqResultY);
		ASSERT_EQ(resultX, SeqResultX);
		ASSERT_EQ(resultY, SeqResultY);
	}
	MPI_Barrier(MPI_COMM_WORLD);
}
TEST(Sobel_Test, Test_4)
{
	MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	int h, w;
	std::pair<int, int> par;
	std::vector<int>image;
	std::vector<int>resultX;
	std::vector<int>resultY;
	//std::cout << "Started Main From " << ProcRank << std::endl;
	if (ProcRank == 0)
	{
		std::random_device dev;
		std::mt19937 gen(dev());
		h = gen() % 100 + ProcSize;
		w = gen() % 100 + ProcSize;
		CreateRandomImage(image, h, w);
		par.first = h;
		par.second = w;
	}
	//std::cout << "Exited" << std::endl;
	//std::cout << "Before Bcast" << std::endl;
	MPI_Bcast(&par, 1, MPI_2INT, 0, MPI_COMM_WORLD);
	//std::cout << "After Bcast" << std::endl;
	//std::cout << par[0] << " " << par[1] << std::endl;
	h = par.first;
	w = par.second;
	//std::cout << "Hello From " << ProcRank << std::endl;
	//MPI_Barrier(MPI_COMM_WORLD);
	CalculateSober(image, h, w, resultX, resultY);
	//MPI_Barrier(MPI_COMM_WORLD);
	if (ProcRank == 0)
	{
		std::vector<int>SeqResultX;
		std::vector<int>SeqResultY;
		SequentalCalculation(image, h, w, SeqResultX, SeqResultY);
		ASSERT_EQ(resultX, SeqResultX);
		ASSERT_EQ(resultY, SeqResultY);
	}
	MPI_Barrier(MPI_COMM_WORLD);
}
TEST(Sobel_Test, Test_5)
{
	MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	int h, w;
	std::pair<int, int> par;
	std::vector<int>image;
	std::vector<int>resultX;
	std::vector<int>resultY;
	//std::cout << "Started Main From " << ProcRank << std::endl;
	if (ProcRank == 0)
	{
		std::random_device dev;
		std::mt19937 gen(dev());
		h = gen() % 100 + ProcSize;
		w = gen() % 100 + ProcSize;
		CreateRandomImage(image, h, w);
		par.first = h;
		par.second = w;
	}
	//std::cout << "Exited" << std::endl;
	//std::cout << "Before Bcast" << std::endl;
	MPI_Bcast(&par, 1, MPI_2INT, 0, MPI_COMM_WORLD);
	//std::cout << "After Bcast" << std::endl;
	//std::cout << par[0] << " " << par[1] << std::endl;
	h = par.first;
	w = par.second;
	//std::cout << "Hello From " << ProcRank << std::endl;
	//MPI_Barrier(MPI_COMM_WORLD);
	CalculateSober(image, h, w, resultX, resultY);
	//MPI_Barrier(MPI_COMM_WORLD);
	if (ProcRank == 0)
	{
		std::vector<int>SeqResultX;
		std::vector<int>SeqResultY;
		SequentalCalculation(image, h, w, SeqResultX, SeqResultY);
		ASSERT_EQ(resultX, SeqResultX);
		ASSERT_EQ(resultY, SeqResultY);
	}
	MPI_Barrier(MPI_COMM_WORLD);
}
int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	MPI_Init(&argc, &argv);

	::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
	::testing::TestEventListeners& listeners =
		::testing::UnitTest::GetInstance()->listeners();

	listeners.Release(listeners.default_result_printer());
	listeners.Release(listeners.default_xml_generator());

	listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
	return RUN_ALL_TESTS();
	/*MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	std::vector<int>image;
	std::vector<int>resultX;
	std::vector<int>resultY;
	std::vector<int>SeqResultX;
	std::vector<int>SeqResultY;
	if (ProcRank == 0)
	{
		CreateRandomImage(image, 10, 10);
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				std::cout << image[i * 10 + j] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		SequentalCalculation(image, 10, 10, SeqResultX, SeqResultY);
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				std::cout << SeqResultX[i * 8 + j] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				std::cout << SeqResultY[i * 8 + j] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	MPI_Barrier(MPI_COMM_WORLD);
	CalculateSober(image, 10, 10, resultX, resultY);
	if (ProcRank == 0) {
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				std::cout << resultX[i * 8 + j] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				std::cout << resultY[i * 8 + j] << " ";
			}
			std::cout << std::endl;
		}
	}
	MPI_Finalize();*/
}