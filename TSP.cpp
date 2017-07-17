#include <vector>
#include <iostream>
#include <thread>
#include <time.h>
#include <stdlib.h>
using namespace std;
//NON-Recursive Version of TSP

//number of nodes (including node 0) (n > 1)
const int n = 7;

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
			sum += adjMat[ comList[i][j] ][ comList[i][j+1] ];
	}
	return sum;
}


int main() {
	srand(time(NULL));

	int adjMat[n][n];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (i != j)
				{adjMat[i][j] = (rand() % 30) + 1;}
			else
				{adjMat[i][j] = 0;}
		}
	}



	clock_t begin = clock();
	//calculate amount of different combos in TSP (n! number of iterations)
	int iters = 1;
	for (int i = 1; i < n; i++) {
		iters *= i;
	}

	//Vector of different combos
	vector<vector<int>> comList(iters);


	//Different Combos 
	for (int i = 1; i < n; i++) {
		com(comList, i, iters);
	}

	//Push 0 in the front and back of list
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


	vector<int> shortestPath;
	int smallestSum = std::numeric_limits<int>::max(); //max int value
	int sum;
	for (unsigned int i = 0; i < comList.size(); i++) {
		//adjancecy matrix, combo list, iteration in combo list,
		sum = DFS(adjMat, comList, i);
		for (unsigned int x = 0; x < comList[i].size(); x++) {
			cout << comList[i][x] << ", ";
		}
		cout << "\tSum: "<<sum<<endl;
		if (smallestSum > sum) {
			smallestSum = sum;
			shortestPath = comList[i];
		}
	}
	cout << "Shortest Path: ";
	for (unsigned int i = 0; i < shortestPath.size(); i++) {
		cout << shortestPath[i] << ", ";
	}
	cout << "\nSum: " << smallestSum << endl;;

	clock_t end = clock();

	cout << "Clock: " << ((float)(end - begin) / CLOCKS_PER_SEC) << endl;

	//Pause
	return 0;
}