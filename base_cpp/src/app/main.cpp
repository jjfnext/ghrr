
#include <iostream>
#include <string>
#include <vector>

#include "fun.hpp"

using namespace std;
using namespace app;

int main()
{
	vector<int> l = {10, 20, 30};

	for (auto i : l) {
//		string txt = run_fun(i);
//		cout << txt;
		cout << run_fun(i) << endl;
	}
	
	return 0;
}

