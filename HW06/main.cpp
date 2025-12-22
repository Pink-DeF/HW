#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <map>
#include <set>
#include <cctype>
#include <utility>

using namespace std;

struct word
{
	string word;
	int weight;
};

string normalize(const string& str)
{
	string result;
	for(char c : str)
	{
		if(!isspace(c)){ result += tolower(c); }
	}
	
	return result;
}

bool isAnagram(const string& fWord, const string& sWord)
{
	return is_permutation(fWord.begin(), fWord.end(), sWord.begin());
}

vector<string> split(const string& sentence)
{
	vector<string> words;
	string word;
	istringstream iss(sentence);

	while(iss >> word)
	{
		transform(word.begin(), word.end(), word.begin(), ::tolower);
		words.push_back(word);
	}
	return words;
}

vector<word> findAnagram(const string& anagram, const vector<word>& dictionary)
{
	vector<word> result;
	for(vector<word>::value_type entry : dictionary)
	{
		if(isAnagram(anagram, entry.word)) { result.push_back(entry); }
	}

	return result;
}


//Немного(очень) теряюсь в рекурсивных функциях, поэтому это сделано при поддержке гпт
void generateDecryptions(
    const vector<string>& anagramWords,
    const vector<vector<word>>& possibleWords,
    size_t index,
    vector<string>& current,
    int totalWeight,
    vector<pair<vector<string>, int>>& results
) {
    if (index == anagramWords.size()) {
        results.emplace_back(current, totalWeight);
        return;
    }
    
    for (const auto& wordInfo : possibleWords[index]) {
        current.push_back(wordInfo.word);
        generateDecryptions(
            anagramWords, 
            possibleWords, 
            index + 1, 
            current, 
            totalWeight + wordInfo.weight, 
            results
        );
        current.pop_back();
    }
}

vector<word> readDictionory(const string& filename)
{
	vector<word> dictionary;
	ifstream file(filename);
	if(!file.is_open()){ throw runtime_error("Cand open file"); }

	string line;
	while(getline(file, line))
	{
		if(line.empty()){ continue; }

		size_t pos = line.find_last_of(" ");
		if(pos == line.length()){ continue; }

		string word = line.substr(0, pos);
		string weightW = line.substr(pos + 1);

		word = normalize(word);
		
		try
		{
			int weight = stoi(weightW);
			dictionary.push_back({word, weight});
		}
		catch(...)
		{
			continue;
		}
	}

	file.close();
	return dictionary;
}

int main(int argc, char* argv[])
{
	if(argc != 3){ return 1; }

	string sentence = argv[1];
	string fileName = argv[2];

	vector<word> dictionary = readDictionory(fileName);
	vector<string> words = split(sentence);

	vector<vector<word>> angWords;

	for(vector<string>::value_type wrd : words)
	{
		vector<word> ang = findAnagram(wrd, dictionary);

		if(ang.empty()){ return 2; }

		angWords.push_back(ang);
	}

	vector<pair<vector<string>, int>> result;
	vector<string> cur;

	generateDecryptions(
            words, 
            angWords, 
            0, 
            cur, 
            0, 
            result
        );

	sort(result.begin(), result.end(),
		[](const pair<vector<string>, int>& a, const pair<vector<string>, int>& b)
		{ return a.second > b.second; });

	for(vector<pair<vector<string>, int>>::value_type res : result)
	{
		//Вывод тоже у гпт. Прочто, чтобы было красиво
        for (size_t i = 0; i < res.first.size(); i++) {
            cout << res.first[i];
            if (i < res.first.size() - 1) {
                cout << " ";
            }
        }
        cout << " (" << res.second << ")" << endl;
	}
}
