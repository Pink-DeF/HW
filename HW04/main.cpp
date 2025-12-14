#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
int main(int argc, char* argv[])
{
	if(argc != 3){ return 1; }

	ifstream inputFile(argv[1]);
	if(!inputFile.is_open())
	{
		cerr << "Cant open file" << endl;
		return 1;
	}

	int n;
	inputFile >> n;

	vector<int> nums(n);
	for(int i = 0; i < n; i++)
	{
		inputFile >> nums[i];
	}
	inputFile.close();

	if(n % 2 != 0 || nums.size() != n){ return 1; }
	//

	sort(nums.begin(), nums.end());
	int minN = accumulate(nums.begin(), nums.begin() + (n / 2), 0);
	int maxN = accumulate(nums.begin() + (n / 2), nums.end(), 0);
	int midN = maxN - minN;

	ofstream outputFile(argv[2]);
	if(!outputFile.is_open())
	{
		cerr << "Cant open file" << endl;
		return 1;
	}

	outputFile << midN;
	outputFile.close();

	return 0;
}
