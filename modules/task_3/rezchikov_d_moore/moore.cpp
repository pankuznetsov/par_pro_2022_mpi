#include "moore.h"



#include <mpi.h>
#include <random>
#include <iostream>

#define INF 10000000

std::vector<int> getRandomGraph(int size){

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> rndRange(1,100);

    std::vector<int> res(size*size);

    for ( int i = 0; i < size; ++i){
        for (int j = 0; j < size; ++j){
            if ( i == j ){
                res[i*size + j] = 0;
            }
            else{
                auto resNum = rndRange(rng);
                
                res[i * size + j] = resNum;
            }
        }
    }

    return res;
}   
void MooreParallel(std::vector<int> & graph, int srcVer, std::vector<int> & res, bool * cycle_flag){
    
    int size, rank, vertexNum;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if ( rank == 0 ){
        vertexNum = sqrt(graph.size());
    } 
    MPI_Bcast(&vertexNum, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if ( srcVer >= vertexNum || srcVer < 0){
        throw std::out_of_range("source vertex not in range");
    }

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int chunkSize = vertexNum / size;
    int rank_begin = chunkSize * rank,
        rank_end = chunkSize * (rank + 1);
    if ( rank == size - 1){
        rank_end = vertexNum;
    }
    std::vector<int> local_g(vertexNum*vertexNum);
    std::vector<int> local_dist(vertexNum);
    if ( rank == 0){
        local_g = std::vector<int>(graph);
    }
    MPI_Bcast(local_g.data(), vertexNum*vertexNum, MPI_INT, 0, MPI_COMM_WORLD);

    local_dist[0] = 0;
    for (int i = 1; i < vertexNum; ++i){
        local_dist[i] = INF;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    
    bool any_change;
    int cnt = 0;
    for ( int i = 0; i < vertexNum; ++i){
        any_change = false;
        cnt++;
        for ( int u = rank_begin; u < rank_end; ++u){
            for ( int v = 0; v < vertexNum; ++v ){
                int w = local_g[u*vertexNum + v]; 
                if ( w < INF ){
                    if ( local_dist[u] + w < local_dist[v]) {
                        local_dist[v] = local_dist[u] + w;
                        any_change = true;
                    }
                }
            }
        }
        MPI_Allreduce(MPI_IN_PLACE, &any_change,
                        1, MPI_CXX_BOOL, MPI_LOR, MPI_COMM_WORLD);
        if (!any_change){
            break;
        }
        if ( cnt == vertexNum ){
            *cycle_flag = true;
            return;
        }
        MPI_Allreduce(MPI_IN_PLACE, local_dist.data(),
                        vertexNum, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
        
    }
    
    if ( rank == 0 ){
        res = std::vector<int>(local_dist);
    }
}
std::vector<int> MooreSequential(std::vector<int> & graph, int srcVer, bool * cycle_flag){
    
    int vertexNum = sqrt(graph.size());
    if ( srcVer >= vertexNum || srcVer < 0 ){
        throw std::out_of_range("source vertex not in range");
    }

    std::vector<int> dist(vertexNum, INT32_MAX);
    dist[srcVer] = 0;


    for ( int i = 0; i < vertexNum - 1; ++i){
        for ( int u = 0; u < vertexNum; ++u){
            for (int v = 0; v < vertexNum; ++v){
                if ( graph[u*vertexNum + v] < INF){
                    if ( dist[v] > dist[u] + graph[u*vertexNum + v]){
                        dist[v] = dist[u] + graph[u*vertexNum + v];
                    }
                }
            }
        }
    }

    for ( int u = 0; u < vertexNum; ++u){
            for (int v = 0; v < vertexNum; ++v){
                if ( graph[u*vertexNum + v] < INF){
                    if ( dist[v] > dist[u] + graph[u*vertexNum + v]){
                        *cycle_flag = true;
                        return graph;
                    }
                }
            }
        }

    return dist;
}