// Copyright 2023 Yablonskii Dmitriy


#include "../../../modules/task_3/yablonskii_d_fox/fox.h"
#include <mpi.h>

#include <random>

namespace {
    MPI_Comm G_C, C_C, R_C;

    void makeGRc(int grs, int p_rank, int* grc) {
        std::vector<int> ds(2, grs);
        std::vector<int> t_per(2, 0);
        std::vector<int> sbd(2);
        MPI_Cart_create(MPI_COMM_WORLD, 2, ds.data(), t_per.data(), 0, &G_C);
        MPI_Cart_coords(G_C, p_rank, 2, grc);
        sbd[0] = 0;
        sbd[1] = 1;
        MPI_Cart_sub(G_C, sbd.data(), &R_C);
        sbd[0] = 1;
        sbd[1] = 0;
        MPI_Cart_sub(G_C, sbd.data(), &C_C);
    }

    void blcM(double* aBl, double* bBl, double* a_b_Bl, size_t Bl_s) {
        for (size_t i = 0; i < Bl_s; i++) {
            for (size_t j = 0; j < Bl_s; j++) {
                double tmp = 0;
                for (int k = 0; k < Bl_s; k++) {
                    tmp += aBl[i * Bl_s + k] * bBl[k * Bl_s + j];
                }
                a_b_Bl[i * Bl_s + j] += tmp;
            }
        }
    }

}  // namespace

namespace myns {
    std::vector<double> creatmat(size_t size) {
        std::mt19937 gen(std::random_device {}());
        std::uniform_real_distribution<> distr(-100, 100);
        std::vector<double> matr(size * size);
        for (size_t i = 0; i < size * size; i++) {
            matr[i] = distr(gen);
        }

        return matr;
    }

    std::vector<double> sM(const std::vector<double>& a, const std::vector<double>& b, size_t size) {
        if (a.size() != b.size()) {
            throw "error";
        }

        std::vector<double> A_B(size * size, 0);
        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < size; j++) {
                for (size_t k = 0; k < size; k++) {
                    A_B[i * size + j] += a[i * size + k] * b[k * size + j];
                }
            }
        }

        return A_B;
    }

    std::vector<double> fM(std::vector<double> a, std::vector<double> b, size_t size) {
        int Num_pc, Rank_pc;
        MPI_Comm_size(MPI_COMM_WORLD, &Num_pc);
        MPI_Comm_rank(MPI_COMM_WORLD, &Rank_pc);

        MPI_Status status;
        int GRi_s = static_cast<int>(sqrt(Num_pc));
        if (GRi_s * GRi_s != Num_pc) {
            throw "error";
        }
        std::vector<int> GRi_c(2);
        makeGRc(GRi_s, Rank_pc, GRi_c.data());
        int Bl_s;
        if (Rank_pc == 0) {
            Bl_s = static_cast<int>(size) / GRi_s;
        }
        MPI_Bcast(&Bl_s, 1, MPI_INT, 0, MPI_COMM_WORLD);
        std::vector<double> bla(Bl_s * Bl_s);
        std::vector<double> blb(Bl_s * Bl_s);
        std::vector<double> bla_b(Bl_s * Bl_s, 0);
        if (Rank_pc == 0) {
            for (int i = 0; i < Bl_s; i++) {
                for (int j = 0; j < Bl_s; j++) {
                    bla[i * Bl_s + j] = a[i * size + j];
                    blb[i * Bl_s + j] = b[i * size + j];
                }
            }
        }
        MPI_Datatype block_mat;
        MPI_Type_vector(Bl_s, Bl_s, Bl_s * GRi_s, MPI_DOUBLE, &block_mat);
        MPI_Type_commit(&block_mat);
        if (Rank_pc == 0) {
            for (int x = 1; x < Num_pc; x++) {
                MPI_Send(a.data() + (x % GRi_s) * Bl_s + (x / GRi_s) * size * Bl_s,
                    1, block_mat, x, 0, G_C);
                MPI_Send(b.data() + (x % GRi_s) * Bl_s + (x / GRi_s) * size * Bl_s,
                    1, block_mat, x, 1, G_C);
            }
        } else {
            MPI_Recv(bla.data(), Bl_s * Bl_s, MPI_DOUBLE,
                     0, 0, G_C, &status);
            MPI_Recv(blb.data(), Bl_s * Bl_s, MPI_DOUBLE,
                     0, 1, G_C, &status);
        }
        for (int i = 0; i < GRi_s; i++) {
            std::vector<double> tmpblockA(Bl_s * Bl_s);
            int p_tt = (GRi_c[0] + i) % GRi_s;
            if (GRi_c[1] == p_tt) {
                tmpblockA = bla;
            }
            MPI_Bcast(tmpblockA.data(), Bl_s * Bl_s,
                      MPI_DOUBLE, p_tt, R_C);
            blcM(tmpblockA.data(), blb.data(), bla_b.data(), Bl_s);
            int N_p = GRi_c[0] + 1;
            if (GRi_c[0] == GRi_s - 1)
                N_p = 0;
            int P_p = GRi_c[0] - 1;
            if (GRi_c[0] == 0)
                P_p = GRi_s - 1;
            MPI_Sendrecv_replace(blb.data(), Bl_s * Bl_s, MPI_DOUBLE,
                                 P_p, 0, N_p, 0, C_C, &status);
        }
        std::vector<double> res(size * size);
        if (Rank_pc == 0) {
            for (int i = 0; i < Bl_s; i++) {
                for (int j = 0; j < Bl_s; j++)
                    res[i * size + j] = bla_b[i * Bl_s + j];
            }
        }
        if (Rank_pc != 0) {
            MPI_Send(bla_b.data(), Bl_s * Bl_s, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
        } else {
            for (int x = 1; x < Num_pc; x++) {
                MPI_Recv(res.data() + (x % GRi_s) * Bl_s + (x / GRi_s) * size * Bl_s,
                    Bl_s * Bl_s, block_mat, x, 3, MPI_COMM_WORLD, &status);
            }
        }
        MPI_Type_free(&block_mat);
        return res;
    }
}  // namespace myns

