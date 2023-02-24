// Copyright 2023 Yunin Daniil
#include <mpi.h>
#include <gtest/gtest.h>
#include <vector>
#include <gtest-mpi-listener.hpp>
#include "./convex_comp_bin.h"

TEST(Convex_Shell_Binary_Image, Test_1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int width = 10, height = 10;
    std::vector<std::vector<int>> global_vec(height, std::vector<int>(width));
    std::vector<int> labeling_image(width*height);
    // std::vector<int> global_vec(7*7);
    // if (rank == 0) {
    //     CreateComponent1(&global_vec);
    //     PrintImage(global_vec, width, height);
    // }
    CreateComponent1(&global_vec);
    labeling_image = Labeling(global_vec, width, height);
    // if (rank == 0) {
    //     // PrintImage(labeling_image, width, height);
    // }
    std::vector<int> convex_shell_par = MainFunParallel(labeling_image, width,
        height, FindNumComponents(labeling_image));
    if (rank == 0) {
        std::vector<int> convex_shell_seq = MainFuncSequence(labeling_image,
            width, height, FindNumComponents(labeling_image));
        // std::cout << "Checked\n";
        // std::cout << "Seq " << convex_shell_seq.size() << " Par " <<  convex_shell_par.size() << "\n";
        // for (int i = 0; i < convex_shell_seq.size(); i++) {
        //     std::cout << "Value seq = " << convex_shell_seq[i] << " Value par " << convex_shell_par[i] << "\n";
        // }
        for (int i = 0; i < convex_shell_seq.size(); i++) {
            ASSERT_EQ(convex_shell_seq[i], convex_shell_par[i]);
        }
    }
}

TEST(Convex_Shell_Binary_Image, Test_2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int width = 4, height = 4;
    std::vector<std::vector<int>> global_vec(height, std::vector<int>(width));
    std::vector<int> labeling_image(width*height);
    // std::vector<int> global_vec(7*7);
    // if (rank == 0) {
    //     CreateComponent1(&global_vec);
    //     PrintImage(global_vec, width, height);
    // }
    CreateComponent2(&global_vec);
    // PrintImage(global_vec, width, height);
    labeling_image = Labeling(global_vec, width, height);
    // PrintImage(labeling_image, width, height);
    std::vector<int> convex_shell_par = MainFunParallel(labeling_image,
        width, height, FindNumComponents(labeling_image));
    if (rank == 0) {
        std::vector<int> convex_shell_seq = MainFuncSequence(labeling_image,
            width, height, FindNumComponents(labeling_image));
        // std::cout << "Checked\n";
        // std::cout << "Seq " << convex_shell_seq.size() << " Par " <<  convex_shell_par.size() << "\n";
        // for (int i = 0; i < convex_shell_seq.size(); i++) {
        //     std::cout << "Value seq = " << convex_shell_seq[i] << " Value par " << convex_shell_par[i] << "\n";
        // }
        for (int i = 0; i < convex_shell_seq.size(); i++) {
            ASSERT_EQ(convex_shell_seq[i], convex_shell_par[i]);
        }
    }
}

TEST(Convex_Shell_Binary_Image, Test_3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int width = 15, height = 15;
    std::vector<std::vector<int>> global_vec(height, std::vector<int>(width));
    std::vector<int> labeling_image(width*height);
    // std::vector<int> global_vec(7*7);
    // if (rank == 0) {
    //     CreateComponent1(&global_vec);
    //     PrintImage(global_vec, width, height);
    // }
    CreateComponent3(&global_vec, width, height);
    // PrintImage(global_vec, width, height);
    labeling_image = Labeling(global_vec, width, height);
    // PrintImage(labeling_image, width, height);
    std::vector<int> convex_shell_par = MainFunParallel(labeling_image,
        width, height, FindNumComponents(labeling_image));
    if (rank == 0) {
        std::vector<int> convex_shell_seq = MainFuncSequence(labeling_image,
            width, height, FindNumComponents(labeling_image));
        // std::cout << "Checked\n";
        // std::cout << "Seq " << convex_shell_seq.size() << " Par " <<  convex_shell_par.size() << "\n";
        // for (int i = 0; i < convex_shell_seq.size(); i++) {
        //     std::cout << "Value seq = " << convex_shell_seq[i] << " Value par " << convex_shell_par[i] << "\n";
        // }
        for (int i = 0; i < convex_shell_seq.size(); i++) {
            ASSERT_EQ(convex_shell_seq[i], convex_shell_par[i]);
        }
    }
}

TEST(Convex_Shell_Binary_Image, Test_4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int width = 7, height = 7;
    std::vector<std::vector<int>> global_vec(height, std::vector<int>(width));
    std::vector<int> labeling_image(width*height);
    // std::vector<int> global_vec(7*7);
    // if (rank == 0) {
    //     CreateComponent1(&global_vec);
    //     PrintImage(global_vec, width, height);
    // }
    CreateComponent4(&global_vec, width, height);
    // PrintImage(global_vec, width, height);
    // std::cout << "\n";
    labeling_image = Labeling(global_vec, width, height);
    // PrintImage(labeling_image, width, height);
    std::vector<int> convex_shell_par = MainFunParallel(labeling_image,
        width, height, FindNumComponents(labeling_image));
    if (rank == 0) {
        std::vector<int> convex_shell_seq = MainFuncSequence(labeling_image,
            width, height, FindNumComponents(labeling_image));
        // std::cout << "Checked\n";
        // std::cout << "Seq " << convex_shell_seq.size() << " Par " <<  convex_shell_par.size() << "\n";
        // for (int i = 0; i < convex_shell_seq.size(); i++) {
        //     std::cout << "Value seq = " << convex_shell_seq[i] << " Value par " << convex_shell_par[i] << "\n";
        // }
        for (int i = 0; i < convex_shell_seq.size(); i++) {
            ASSERT_EQ(convex_shell_seq[i], convex_shell_par[i]);
        }
    }
}

TEST(Convex_Shell_Binary_Image, Test_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int width = 5, height = 5;
    std::vector<std::vector<int>> global_vec(height, std::vector<int>(width));
    std::vector<int> labeling_image(width*height);
    // std::vector<int> global_vec(7*7);
    // if (rank == 0) {
    //     CreateComponent1(&global_vec);
    //     PrintImage(global_vec, width, height);
    // }
    CreateComponent5(&global_vec, width, height);
    // PrintImage(global_vec, width, height);
    // std::cout << "\n";
    labeling_image = Labeling(global_vec, width, height);
    // PrintImage(labeling_image, width, height);
    std::vector<int> convex_shell_par = MainFunParallel(labeling_image,
        width, height, FindNumComponents(labeling_image));
    if (rank == 0) {
        std::vector<int> convex_shell_seq = MainFuncSequence(labeling_image,
            width, height, FindNumComponents(labeling_image));
        // std::cout << "Checked\n";
        // std::cout << "Seq " << convex_shell_seq.size() << " Par " <<  convex_shell_par.size() << "\n";
        // for (int i = 0; i < convex_shell_seq.size(); i++) {
        //     std::cout << "Value seq = " << convex_shell_seq[i] << " Value par " << convex_shell_par[i] << "\n";
        // }
        for (int i = 0; i < convex_shell_seq.size(); i++) {
            ASSERT_EQ(convex_shell_seq[i], convex_shell_par[i]);
        }
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
