#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>

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

	int N, a, b;
	inputFile >> N >> a >> b;

	vector<int> nums(N);
	for(int i = 0; i < N;i++)
	{
		inputFile >> nums[i];
	}

	inputFile.close();
	if(nums.size() != N){ return 1; }

	vector<int> s_nums = nums;

	int maxN = max(a,b);
	int minN = min(a,b);
	auto it = remove_if(nums.begin(), nums.end(), [maxN, minN](int n)
			{ return n > maxN || n < minN; } );
	nums.erase(it, nums.end());

	vector<int> d_nums = nums;

	auto max_it = max_element(nums.begin(), nums.end());
	auto t_it = find_if(next(max_it), nums.end(), [](int n) { return n % 2 == 0; });

	if(t_it != nums.end()){ nums.erase(t_it); }

	size_t positive_count = count_if(nums.begin(), nums.end(), [](int n){ return n > 0; });
	size_t negative_count = count_if(nums.begin(), nums.end(), [](int n){ return n < 0; });
	
	ofstream outputFile(argv[2]);
	if(!outputFile.is_open())
	{
		cerr << "Cant open file" << endl;
		return 1;
	}

	ostream_iterator<int> out_it(outputFile, " ");
	copy(s_nums.begin(), s_nums.end(), out_it);
	outputFile << "\n";
	copy(d_nums.begin(), d_nums.end(), out_it);
	outputFile << "\n";
	copy(nums.begin(), nums.end(), out_it);
	outputFile << "\n";

	outputFile << "Число положительных чисел:" << positive_count << "\n";
	outputFile << "Число отрицательных чисел:" << negative_count << "\n";

	outputFile.close();

	return 0;
}
