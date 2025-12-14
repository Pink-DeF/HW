#include <iostream>
#include <variant>
#include <utility>
#include <cmath>
#include <string>
#include <optional>

using namespace std;

using v = variant<monostate, double, pair<double, double>>;

v calc(double a, double b, double c)
{
	double D = b * b - 4 * a * c;
	if(D < 0){ return monostate{}; }
	else if(D == 0){ return (double)(( -b) / ( 2 * a)); }
	else { return make_pair( (double) (-b + sqrt(D)) / (2 * a), (double)(-b - sqrt(D)) / (2 * a) ); };
}

class Visitor
{
public:
	void operator()(monostate) const { cout << "Нет действительных корней (((" << endl; }
	void operator()(double root) const { cout << "Один корень: " << root << endl; }
	void operator()(const pair<double, double>& roots) const { cout << "Два корня: " << roots.first << " " << roots.second << endl; }
};

optional<int> pars (int argc, char* argv[])
{
	int sum = 0;
	bool valid = false;

	for(int i = 1; i < argc; i++)
	{
		string arg = argv[i];
		try
		{
			size_t pos = 0;
			int val = stoi(arg, &pos);

			if(pos == arg.size())
			{
				sum += val;
				valid = true;
			}
		}
		catch(...)
		{
			continue;
		}
	}

	return valid ? optional<int>(sum) : nullopt;
}

int main(int argc, char* argv[])
{
	auto  res = calc(2.0, -5.0, 2.0);
	visit(Visitor{}, res);

	auto sum = pars(argc, argv);
	if(sum) { cout << *sum << endl; }

	return 0;
}
