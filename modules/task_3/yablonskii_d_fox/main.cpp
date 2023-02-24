// Copyright 2023 Yablonskii Dmitriy


#include <gtest/gtest.h>
#include <cmath>
#include "../../../modules/task_3/yablonskii_d_fox/fox.h"
#include <gtest-mpi-listener.hpp>

TEST(Test_algfox, MatX1) {
    int Rank_pc, Num_pc;
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank_pc);
    MPI_Comm_size(MPI_COMM_WORLD, &Num_pc);
    size_t size = 1;
    int BLK = static_cast<int>(sqrt(Num_pc));
    if (BLK * BLK == Num_pc) {
        std::vector<double> a;
        std::vector<double> b;
        std::vector<double> ab_fox;
        int dob = size % BLK;
        if (dob) {
            size += BLK - (size % BLK);
        }
        if (Rank_pc == 0) {
            a = myns::creatmat(size);
            b = myns::creatmat(size);
        }
        ab_fox = myns::fM(a, b, size);
        if (Rank_pc == 0) {
            std::vector<double> ab_seq = myns::sM(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

TEST(Test_algfox, MatX5) {
    int Rank_pc, Num_pc;
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank_pc);
    MPI_Comm_size(MPI_COMM_WORLD, &Num_pc);
    size_t size = 5;
    int BLK = static_cast<int>(sqrt(Num_pc));
    if (BLK * BLK == Num_pc) {
        std::vector<double> a;
        std::vector<double> b;
        std::vector<double> ab_fox;
        int dob = size % BLK;
        if (dob) {
            size += BLK - (size % BLK);
        }
        if (Rank_pc == 0) {
            a = myns::creatmat(size);
            b = myns::creatmat(size);
        }
        ab_fox = myns::fM(a, b, size);
        if (Rank_pc == 0) {
            std::vector<double> ab_seq = myns::sM(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

TEST(Test_algfox, MatX10) {
    int Rank_pc, Num_pc;
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank_pc);
    MPI_Comm_size(MPI_COMM_WORLD, &Num_pc);
    size_t size = 10;
    int BLK = static_cast<int>(sqrt(Num_pc));
    if (BLK * BLK == Num_pc) {
        std::vector<double> a;
        std::vector<double> b;
        std::vector<double> ab_fox;
        int dob = size % BLK;
        if (dob) {
            size += BLK - (size % BLK);
        }
        if (Rank_pc == 0) {
            a = myns::creatmat(size);
            b = myns::creatmat(size);
        }
        ab_fox = myns::fM(a, b, size);
        if (Rank_pc == 0) {
            std::vector<double> ab_seq = myns::sM(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

TEST(Test_algfox, MatX20) {
    int Rank_pc, Num_pc;
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank_pc);
    MPI_Comm_size(MPI_COMM_WORLD, &Num_pc);
    size_t size = 20;
    int BLK = static_cast<int>(sqrt(Num_pc));
    if (BLK * BLK == Num_pc) {
        std::vector<double> a;
        std::vector<double> b;
        std::vector<double> ab_fox;
        int dob = size % BLK;
        if (dob) {
            size += BLK - (size % BLK);
        }
        if (Rank_pc == 0) {
            a = myns::creatmat(size);
            b = myns::creatmat(size);
        }
        ab_fox = myns::fM(a, b, size);
        if (Rank_pc == 0) {
            std::vector<double> ab_seq = myns::sM(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

TEST(Test_algfox, MatX50) {
    int Rank_pc, Num_pc;
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank_pc);
    MPI_Comm_size(MPI_COMM_WORLD, &Num_pc);
    size_t size = 50;
    int BLK = static_cast<int>(sqrt(Num_pc));
    if (BLK * BLK == Num_pc) {
        std::vector<double> a;
        std::vector<double> b;
        std::vector<double> ab_fox;
        int dob = size % BLK;
        if (dob) {
            size += BLK - (size % BLK);
        }
        if (Rank_pc == 0) {
            a = myns::creatmat(size);
            b = myns::creatmat(size);
        }
        ab_fox = myns::fM(a, b, size);
        if (Rank_pc == 0) {
            std::vector<double> ab_seq = myns::sM(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

TEST(Test_algfox, MatX100) {
    int Rank_pc, Num_pc;
    MPI_Comm_rank(MPI_COMM_WORLD, &Rank_pc);
    MPI_Comm_size(MPI_COMM_WORLD, &Num_pc);
    size_t size = 100;
    int BLK = static_cast<int>(sqrt(Num_pc));
    if (BLK * BLK == Num_pc) {
        std::vector<double> a;
        std::vector<double> b;
        std::vector<double> ab_fox;
        int dob = size % BLK;
        if (dob) {
            size += BLK - (size % BLK);
        }
        if (Rank_pc == 0) {
            a = myns::creatmat(size);
            b = myns::creatmat(size);
        }
        ab_fox = myns::fM(a, b, size);
        if (Rank_pc == 0) {
            std::vector<double> ab_seq = myns::sM(a, b, size);
            for (int i = 0; i < size * size; i++) {
                ASSERT_NEAR(ab_fox[i], ab_seq[i], 0.0001);
            }
        }
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
