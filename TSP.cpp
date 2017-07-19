#include <vector>
#include <iostream>
#include <omp.h>
#include <mpi.h>
#include <time.h>
using namespace std;

const int seed = 150;
const int n = 8;
const int smallestValue = 30;

void com( vector < vector < int > > &list, int n, int iters){
    for(int i = 0; i < iters; i++){
        vector<int>::iterator iter =list[i].begin();
        list[i].insert(iter + (i % n), n);
    }
}

int DFS(int adjMat[n][n], vector < vector < int > > comList, int i){
    int sum = 0;
    for(int j = 0; j < n; j++){
        sum += adjMat[ comList[i][j] ][ comList[i][j + 1] ];
    }
    return sum;
}

int main(int argc, char * argv[]){
    MPI_Init(&argc, &argv);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    int *sums;
    if(world_rank == 0){
        sums = (int *)malloc(sizeof(int) * world_size);
    }
    srand( seed );

    int adjMat[n][n];
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(i != j)
                {adjMat[i][j] = (rand() % smallestValue) + 1;}
            else
                {adjMat[i][j] = 0;}
        }
    }

    int tid, nthreads;
    vector<int> shortestPath;
    int smallestSum = (smallestValue * (n + 1)) + 1;
    int sum;

    int iters = 1;
    for(int i = 1; i < n; i++)
        {iters *= i;}
    vector< vector < int > > comList(iters);

    for(int i = 1; i < n; i++)
        {com(comList, i, iters);}
    
    for(int i = 0; i < comList.size(); i++){
        vector<int>::iterator it = comList[i].begin();
        comList[i].insert(it, 0);
        comList[i].push_back(0);
    }
#pragma omp parallel
{
    tid = omp_get_thread_num();
    nthreads = omp_get_num_threads();
    for(int i = (nthreads * world_rank) + tid; i < comList.size(); i += (nthreads * world_size)){
        sum = DFS(adjMat, comList, i);
        if(smallestSum > sum){
            smallestSum = sum;
            shortestPath = comList[i];
        }

    }

}//end of parallel omp 
    unsigned int lowest = n;
    MPI_Gather(&smallestSum, 1, MPI_INT, sums, 1 , MPI_INT, 0, MPI_COMM_WORLD);
    int sSOA = 30 * (n + 1) + 1; //smallestSumOfAll
    if(world_rank == 0){
    for(int i = 0; i < world_size; i++){
        if(sums[i] < sSOA){
            lowest = i;
            sSOA = sums[i];
        }
    }   
        for(int i = 1; i < world_size; i++){
            MPI_Send(&lowest, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
else{MPI_Recv(&lowest, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);}
    
    if(world_rank == lowest){
        cout<<"Lowest Sum: "<<smallestSum<<endl;
        cout<<"Shorest Path: ";
        for(int i = 0; i < shortestPath.size(); i++){
            cout << shortestPath[i] <<", ";
        }
        cout<<"\nFrom Rank: "<<world_rank<<endl;
        }
    MPI_Finalize();
    return 0;
}
