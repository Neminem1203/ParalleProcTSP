#include <iostream>
#include <vector>
using namespace std;
const int m = std::numeric_limits<int>::max(); //max int value
const int n = 4;

vector<int> DFS(int i,int j, int &sum) {
	vector<int> tour(0, 1);

	while (tour.size() != n) {

	}
	tour.push_back(0);
	return tour;
}

void combo(vector<vector<int>> &possibleCombos, int n, int iters) {
	for (int i = 0; i < iters; i++) {
	}
}


int main_() {
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
	int iters = 1;
	for (int i = 2; i < n; i++) {
		iters = iters * i;
	}
	vector<vector<int>> possibleCombos(n);
	for (int i = 0; i < n; i++) {
		possibleCombos[i].resize(n);
	}


	
	vector<int> city_visited;
	vector<int> shortestPath(0, 1);
	int smallestSum = m;
	int sum;

	/*
	city_visited = DFS(i, j, sum);
	if (smallestSum > sum) {
		shortestPath = city_visited;
		smallestSum = sum;
	}*/

	std::cin.ignore();
	return 0;
}