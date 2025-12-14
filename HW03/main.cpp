#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

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

	string line;
	getline(inputFile, line);
	inputFile.close();

	size_t count0 = 0;
	size_t count1 = 0;

	if(!line.empty())
	{
		stringstream ss(line);
		vector<int> nums;
		int num;

		while ( ss >> num){ nums.push_back(num); }

		auto last = unique(nums.begin(), nums.end());
		nums.erase(last, nums.end());

		count0 = count(nums.begin(), nums.end(), 0);
		count1 = count(nums.begin(), nums.end(), 1);
	}

	ofstream outputFile(argv[2]);
	if(!outputFile.is_open())
	{
		cerr << "Cant open file" << endl;
		return 1;
	}

	outputFile << count1 << " " << count0;
	outputFile.close();

	return 0;
}
