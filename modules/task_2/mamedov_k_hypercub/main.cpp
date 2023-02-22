#include<gtest-mpi-listener.hpp>
#include<gtest/gtest.h>
#include<iostream>
#include<mpi.h>
#include"hypercub.h"
int ProcRank, ProcNum;
TEST(Hyper_Cub_Test, Test_From_0_To_3)
{
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int>data(10);
    std::vector<int>path;
    if (isHyperCube(ProcNum))
    {
        if (ProcRank == 0)
        {
            GenerateData(data, 10);
            std::cout << "Data on sending process:";
            for (int i = 0; i < 10; i++)
            {
                std::cout << data[i] << " ";
            }
            std::cout << std::endl;
        }
        SendData(data, 10, 0, 3, path);
        if (ProcRank == 3)
        {
            std::cout << "Path from 0 to 3:";
            for (auto x : path)
            {
                std::cout << x << " ";
            }
            std::cout << std::endl;
            std::cout << "Data on receiving proccess:";
            for (auto x : data)
            {
                std::cout << x << " ";
            }
            std::cout << std::endl;

        }
    }
    else if (ProcRank == 0)
    {
        std::cout << "Number of proccess must be square of 2!!!" << std::endl;
    }
}
TEST(Hyper_Cub_Test, Test_From_0_To_2)
{
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int>data(10);
    std::vector<int>path;
    if (isHyperCube(ProcNum))
    {
        if (ProcRank == 0)
        {
            GenerateData(data, 10);
            std::cout << "Data on sending process:";
            for (int i = 0; i < 10; i++)
            {
                std::cout << data[i] << " ";
            }
            std::cout << std::endl;
        }
        SendData(data, 10, 0, 2, path);
        if (ProcRank == 2)
        {
            std::cout << "Path from 0 to 2:";
            for (auto x : path)
            {
                std::cout << x << " ";
            }
            std::cout << std::endl;
            std::cout << "Data on receiving proccess:";
            for (auto x : data)
            {
                std::cout << x << " ";
            }
            std::cout << std::endl;

        }
    }
    else if (ProcRank == 0)
    {
        std::cout << "Number of proccess must be square of 2!!!" << std::endl;
    }
}
TEST(Hyper_Cub_Test, Test_From_0_To_1)
{
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int>data(10);
    std::vector<int>path;
    if (isHyperCube(ProcNum))
    {
        if (ProcRank == 0)
        {
            GenerateData(data, 10);
            std::cout << "Data on sending process:";
            for (int i = 0; i < 10; i++)
            {
                std::cout << data[i] << " ";
            }
            std::cout << std::endl;
        }
        SendData(data, 10, 0, 1, path);
        if (ProcRank == 1)
        {
            std::cout << "Path from 0 to 1:";
            for (auto x : path)
            {
                std::cout << x << " ";
            }
            std::cout << std::endl;
            std::cout << "Data on receiving proccess:";
            for (auto x : data)
            {
                std::cout << x << " ";
            }
            std::cout << std::endl;

        }
    }
    else if (ProcRank == 0)
    {
        std::cout << "Number of proccess must be square of 2!!!" << std::endl;
    }
}
TEST(Hyper_Cub_Test, Test_From_1_To_3)
{
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int>data(10);
    std::vector<int>path;
    if (isHyperCube(ProcNum))
    {
        if (ProcRank == 1)
        {
            GenerateData(data, 10);
            std::cout << "Data on sending process:";
            for (int i = 0; i < 10; i++)
            {
                std::cout << data[i] << " ";
            }
            std::cout << std::endl;
        }
        SendData(data, 10, 1, 3, path);
        if (ProcRank == 3)
        {
            std::cout << "Path from 1 to 3:";
            for (auto x : path)
            {
                std::cout << x << " ";
            }
            std::cout << std::endl;
            std::cout << "Data on receiving proccess:";
            for (auto x : data)
            {
                std::cout << x << " ";
            }
            std::cout << std::endl;

        }
    }
    else if (ProcRank == 0)
    {
        std::cout << "Number of proccess must be square of 2!!!" << std::endl;
    }
}
TEST(Hyper_Cub_Test, Test_From_1_To_2)
{
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int>data(10);
    std::vector<int>path;
    if (isHyperCube(ProcNum))
    {
        if (ProcRank == 1)
        {
            GenerateData(data, 10);
            std::cout << "Data on sending process:";
            for (int i = 0; i < 10; i++)
            {
                std::cout << data[i] << " ";
            }
            std::cout << std::endl;
        }
        SendData(data, 10, 1, 2, path);
        if (ProcRank == 2)
        {
            std::cout << "Path from 1 to 2:";
            for (auto x : path)
            {
                std::cout << x << " ";
            }
            std::cout << std::endl;
            std::cout << "Data on receiving proccess:";
            for (auto x : data)
            {
                std::cout << x << " ";
            }
            std::cout << std::endl;

        }
    }
    else if (ProcRank == 0)
    {
        std::cout << "Number of proccess must be square of 2!!!" << std::endl;
    }
}
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());
    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}