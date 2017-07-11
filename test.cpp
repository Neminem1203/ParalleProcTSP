#include <vector>
#include <iostream>
using namespace std;
const int n = 4;


void com(vector<vector<int>> &test, int n, int iters) {
	for (int i = 0; i < iters; i++) {
		vector<int>::iterator iter = test[i].begin();
		test[i].insert(iter + (i%n), n);
	}
}


int main() {
	int iters = 1;
	for (int i = 1; i < n; i++) {
		iters *= i;
	}

	vector<vector<int>> test(iters);
	
	for (int i = 1; i < n; i++) {
		com(test, i, iters);
	}

	
	for (int i = 0; i < test.size(); i++) {
		for (int j = 0; j < test[i].size(); j++) {
			cout << test[i][j] << " ";
		}
		cout << endl;
	}
	cin.ignore();
}