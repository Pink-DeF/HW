#include <iostream>
#include <string>
#include <stack>
#include <map>

using namespace std;
int main()
{
	string input;
	getline(cin, input);

	stack<pair<char, size_t>> st;
	map<char, char> matching_bracket = {
		{')', '('},
		{']', '['},
		{'}', '{'}
	};

	for(size_t i = 0; i < input.size(); i++)
	{
		char ch = input[i];
		
		if(ch == '(' || ch == '{' || ch == '[') { st.push({ch, i + 1}); }
		else if(ch == ')' || ch == '}' || ch == ']')
		{
			if(st.empty() || st.top().first != matching_bracket[ch])
			{
				cout<< i + 1 << endl;
				return 0;
			}
			st.pop();
		}

	}

	if(st.empty())
	{
		cout<< "Success" <<endl;
	}
	else
	{
		size_t unmatch;
		while(!st.empty())
		{
			unmatch = st.top().second;
			st.pop();
		}
		cout<< unmatch <<endl;
	}

	return 0;
}
