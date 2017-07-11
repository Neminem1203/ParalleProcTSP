#include <vector>
#include <iostream>
using namespace std;

//number of nodes (including node 0) (n > 1)
const int n = 4;


//calculate different combos
void com(vector<vector<int>> &test, int n, int iters) {
	for (int i = 0; i < iters; i++) {
		vector<int>::iterator iter = test[i].begin();
		test[i].insert(iter + (i%n), n);
	}
}


int main() {

	//calculate amount of different combos in TSP
	int iters = 1;
	for (int i = 1; i < n; i++) {
		iters *= i;
	}

	//Vector of different combos
	vector<vector<int>> test(iters);
	
	//Different Combos 
	for (int i = 1; i < n; i++) {
		com(test, i, iters);
	}

	
	//Output
	for (int i = 0; i < test.size(); i++) {
		for (int j = 0; j < test[i].size(); j++) {
			cout << test[i][j] << " ";
		}
		cout << endl;
	}
	//Pause
	cin.ignore();
	return 0;
}