#include <vector>
#include <iostream>
#include <thread>
#include <time.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>
using namespace std;
//NON-Recursive Version of TSP

//number of nodes (including node 0) (n > 1)
const int n = 4;

//Matrix in textbook
/*	int adjMat[n][n] = {
{ 0, 1, 3, 8 },
{ 5, 0, 2, 6 },
{ 1,18, 0,10 },
{ 7, 4,12, 0 }
};
*/

//calculate different combos (creating combo list)
void com(vector<vector<int>> &list, int n, int iters) {
	for (int i = 0; i < iters; i++) {
		vector<int>::iterator iter = list[i].begin();
		list[i].insert(iter + (i%n), n);
	}
}
//adjacency matrix, combination list		, iteration of combo list, this path 
int DFS(int adjMat[n][n], vector<vector<int>> comList, int i) {
	int sum = 0;
	for (int j = 0; j < n; j++) {
		sum += adjMat[comList[i][j]][comList[i][j + 1]];
	}
	return sum;
}


int main() {
	MPI_Init(NULL, NULL);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int *sums = (int *)malloc(sizeof(int) * world_size);
	vector<int> *paths = (vector<int> *)malloc(sizeof(vector<int>) * world_size);
	MPI_Comm comm;
	srand(150);

	int adjMat[n][n];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i != j)
			{
				adjMat[i][j] = (rand() % 30) + 1;
			}
			else
			{
				adjMat[i][j] = 0;
			}
		}
	}

	printf("rank %d out of %d processors\n", world_rank, world_size);
	int tid, nthreads;
	clock_t begin = clock();
	vector<int> shortestPath;
	int smallestSum = std::numeric_limits<int>::max(); //max int value
	int sum;
#pragma omp parallel private(nthreads, tid)
	{
		tid = omp_get_thread_num();
		nthreads = omp_get_num_threads();
		//calculate amount of different combos in TSP (n! number of iterations)

		int iters = 1;
		for (int i = 1; i < n; i++) {
			iters *= i;
		}

		//Vector of different combos
		vector<vector<int>> comList(iters);


		//Different Combinations 
		for (int i = 1; i < n; i++) {
			com(comList, i, iters);
		}

		//Push 0 in the front and back of list (we want to leave and reeneter the 0th node)
		for (unsigned int i = 0; i < comList.size(); i++) {
			vector<int>::iterator it = comList[i].begin();
			comList[i].insert(it, 0);
			comList[i].push_back(0);
		}

		//Output
		/*
		for (int i = 0; i < comList.size(); i++) {
		for (int j = 0; j < comList[i].size(); j++) {
		cout << comList[i][j] << " ";
		}
		cout << endl;
		}*/



		for (unsigned int i = (nthreads * world_rank) + tid ; i < comList.size(); i += (nthreads * world_size)) {
			//adjancecy matrix, combo list, iteration in combo list,
			sum = DFS(adjMat, comList, i);
			/*
			for (unsigned int x = 0; x < comList[i].size(); x++) {
			cout << comList[i][x] << ", ";
			}
			cout << "\tSum: " << sum << endl;*/
			if (smallestSum > sum) {
				smallestSum = sum;
				shortestPath = comList[i];
			}
		}
		printf("Thread %d of %d completed\n", tid, nthreads);
	}
	MPI_Gather(&sums, 1, MPI_INT, &smallestSum, 1, MPI_INT, MPI_COMM_WORLD,comm);
	int lowest = std::numeric_limits<int>::max();
	if (world_rank == 0) {
		int sumOfAll = std::numeric_limits<int>::max(); //max int value
		
		for (int i = 0; i < world_rank; i++) {
			//TODO: Check if sumOfAll > sum[i] and then make 
			if (sums[i] < sumOfAll) {
				lowest = i;
				sumOfAll = sums[i];
			}
		}

	cout << "\nLowest Sum: " << sumOfAll << endl;;
	}
	if(world_rank == lowest){
	cout << "Shortest Path: ";
	for (unsigned int i = 0; i < shortestPath.size(); i++) {
		cout << shortestPath[i] << ", ";
	}
	}

	clock_t end = clock();
	cout << "Clock: " << ((float)(end - begin) / CLOCKS_PER_SEC) << endl;

	MPI_Finalize();
	cin.ignore();
	//Pause
	return 0;
}