#include <iostream>
#include <vector>
using namespace std;
const int m = std::numeric_limits<int>::max(); //max int value
const int n = 4;

//go from node 0 through the different combos and back to node 0
vector<int> DFS(int i,int j, int &sum) {
	vector<int> tour(0, 1);

	while (tour.size() != n) {

	}
	tour.push_back(0);
	return tour;
}

//take code from test.cpp to create different combo
void combo(vector<vector<int>> &possibleCombos, int n, int iters) {
	for (int i = 0; i < iters; i++) {
	}
}


int main_() {
	//test matrix (TODO: real matrix input)
	int adjMat[n][n] = {
		{0, 1, 3, 8},
		{5, 0, 2, 6},
		{1,18, 0,10},
		{7, 4,12, 0}
	};
	/*
	//prints the adjacency matrix
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << adjMat[i][j] << " ";
		}
		std::cout << std::endl;
	}
	*/

	//calculate amount of different combos in TSP
	int iters = 1;
	for (int i = 2; i < n; i++) {
		iters = iters * i;
	}
	//Vector of possible combos (TODO: Copy from test.cpp
	vector<vector<int>> possibleCombos(n);
	for (int i = 0; i < n; i++) {
		possibleCombos[i].resize(n);
	}


	//TODO: actual DFS Code
	vector<int> city_visited;
	vector<int> shortestPath(0, 1);
	int smallestSum = m;
	int sum;

	/*
	//TODO: idk
	city_visited = DFS(i, j, sum);
	if (smallestSum > sum) {
		shortestPath = city_visited;
		smallestSum = sum;
	}*/

	//pause
	std::cin.ignore();
	return 0;
}