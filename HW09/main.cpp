#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <sstream>

//Написал все сам, но для красоты взял рефернс у гпт
using namespace std;
class Display
{
private:
	//Это тоже украдено у гпт >_<
	void clearLines(int lines)
	{
		for(int i = 0; i < lines; i++){ cout << "\033[A\033[2K"; }
	}

public:
	Display(size_t numTasks): totalTasks(numTasks), progressBars(numTasks){ for(auto& p: progressBars){ p = 0; } }

	void updateProgress(size_t taskId, size_t progress)
	{
		lock_guard<mutex> lock(mtx);
		progressBars[taskId] = progress;
		redisplay();
	}
	void taskCompleted(){ completed++; }

	//Укарал у гпт вывод в консоль, чтобы было красиво
	void redisplay() {
		clearLines(totalTasks + 1);
		
		cout << "Общий прогресс: [" << string(completed, '#') 
			  << string(totalTasks - completed, ' ') << "] "
			  << (completed * 100 / totalTasks) << "%\n";
		
		for (int i = 0; i < totalTasks; ++i) {
		    int progress = progressBars[i];
		    int barWidth = 50;
		    int pos = barWidth * progress / 100;
		    
		    cout << "Задача " << setw(2) << i + 1 << ": [";
		    for (int j = 0; j < barWidth; ++j) {
			if (j < pos) cout << "=";
			else if (j == pos) cout << ">";
			else cout << " ";
		    }
		    cout << "] " << setw(3) << progress << "%\n";
		}
		cout.flush();
    	}

	bool allComplited() { return completed >= totalTasks; }

private:
	mutex mtx;
	size_t totalTasks;

	atomic<size_t> completed{0};
	vector<atomic<size_t>> progressBars;
};

void Task(size_t ID, size_t duration, Display& display)
{
	size_t steps = 100;

	for(size_t i = 0; i <= steps; i++)
	{
		this_thread::sleep_for(chrono::milliseconds(duration * 1000 / 100));
		display.updateProgress(ID, i);
	}
	display.taskCompleted();
}

int main(int argc, char* argv[])
{
	if(argc != 2){ return 1; }
	size_t c_task = 0;

	try{ c_task = static_cast<size_t>(stoul(argv[1])); }
	catch(...){ return 1; }

	vector<thread> threads;
	Display display(c_task);

	for (size_t i = 0; i < c_task; ++i)
	{
		size_t duration = 3 + (i % 3);
		threads.emplace_back(Task, i, duration, ref(display));
	}

	while (!display.allComplited())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	for (auto& t : threads)
	{
		t.join();
	}

	std::cout << "\nВсе задачи выполнены!\n";

	return 0;
}
