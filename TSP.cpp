#include <vector>
#include <iostream>
#include <omp.h>
#include <mpi.h>
#include <time.h>
using namespace std;

const int seed = time(NULL); //use a constant number for constant results
const int n = 8; //number of nodes > 2 and nodes < 9
const int greatestValue= 30; //greatest value of srand

//makes all different combinations
void com( vector < vector < int > > &list, int n, int iters){
    for(int i = 0; i < iters; i++){
        vector<int>::iterator iter =list[i].begin();
        list[i].insert(iter + (i % n), n);
    }
}

//DFS based on ith element of the combination list
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
    printf("Node %d begin\n", world_rank);
    //the sums that will be gathered at the end of the program
    int *sums;
    if(world_rank == 0){
        sums = (int *)malloc(sizeof(int) * world_size);
    }
    //creates random number. change the seed at the top of the file
    srand( seed );

    int adjMat[n][n];
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(i != j)
                {adjMat[i][j] = (rand() % greatestValue) + 1;}
            else
                {adjMat[i][j] = 0;}
        }
    }
    
    //thread id and number of threads
    int tid, nthreads;
    //shortest Path, smallest Sum, and sum used during OMP
    vector<int> shortestPath;
    int smallestSum = (greatestValue * (n + 1)) + 1;
    int sum;
    
    //Checks how many different iterations of combinations we have
    int iters = 1;
    for(int i = 1; i < n; i++)
        {iters *= i;}
    vector< vector < int > > comList(iters);
    
    //creates the combination list
    for(int i = 1; i < n; i++)
        {com(comList, i, iters);}
    
    //push 0 in the front and back because we want to start and end at node 0
    for(unsigned int i = 0; i < comList.size(); i++){
        vector<int>::iterator it = comList[i].begin();
        comList[i].insert(it, 0);
        comList[i].push_back(0);
    }

//OMP
#pragma omp parallel
{
    //tid = thread id   nthreads = number of threads
    tid = omp_get_thread_num();
    nthreads = omp_get_num_threads();
    for(unsigned int i = (nthreads * world_rank) + tid; i < comList.size(); i += (nthreads * world_size)){
        //find sum and check if it's smaller than the current smallestSum.
        //if smaller, set smallestSum equal to it and set shortestPath vector to comList[i]
        sum = DFS(adjMat, comList, i);
        if(smallestSum > sum){
            smallestSum = sum;
            shortestPath = comList[i];
        }

    }

}//end of parallel omp 
    //the rank with the lowest node (set to n for now)
    int lowest = n;
    //Gather all the smallestSums from each node and put it into sums
    MPI_Gather(&smallestSum, 1, MPI_INT, sums, 1 , MPI_INT, 0, MPI_COMM_WORLD);
    int sSOA = 30 * (n + 1) + 1; //smallestSumOfAll
    if(world_rank == 0){
        //check which is the smallest sum of all of the nodes
        for(int i = 0; i < world_size; i++){
            if(sums[i] < sSOA){
                lowest = i;
                sSOA = sums[i];
            }
        }
        //print out the adjMat
        
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cout<<adjMat[i][j]<<"\t";
            }
            cout<<"\n";
        }
        cout<<"\n";
        //send to all the nodes which of the nodes contain the lowest sum
        for(int i = 1; i < world_size; i++){
            MPI_Send(&lowest, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }//if world_rank != 0, they will receive a message which contains the
     //the node with the lowest number
else{MPI_Recv(&lowest, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);}
    
    //if this node is the one with the lowest sum, print it out with the shortest Path
    if(world_rank == lowest){
        cout<<"Lowest Sum: "<<smallestSum<<endl;
        cout<<"Shorest Path: ";
        for(unsigned int i = 0; i < shortestPath.size(); i++){
            cout << shortestPath[i] <<", ";
        }
        cout<<"\nFrom Rank: "<<world_rank<<endl;
        }
    //end MPI
    MPI_Finalize();
    return 0;
}
