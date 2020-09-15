#pragma once

#include <chrono>
#include <vector>
#include <iostream>
#include <thread>

class Timer
{
public:

	Timer() : m_beg(clock_t::now()) {}

	void Reset()
	{
		m_beg = clock_t::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}

private:

	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1>>;

	std::chrono::time_point<clock_t> m_beg;
};

class Performance
{
public:

	void Run(int NumOfTests, void(*TestCode)(), long long IntervalInMilliSeconds = 1000);

private:
	Timer T;

	double SingleTestTime;
	double TotalTime;
	double AvgTPerFcn;

	std::vector<double> Results;

	void PrintDescription(int NumOfTests, long long IntervalInSeconds);
	void PrintResults(int NTests);

};

//runs full test using a fcnptr to refer to user code
void Performance::Run(int NumOfTests, void(*TestCode)(), long long IntervalInSeconds)
{
	PrintDescription(NumOfTests, IntervalInSeconds);

	TotalTime = 0;
	for (int i{ 0 }; i < NumOfTests; ++i)
	{
		T.Reset(); //start timer from zero
		TestCode();
		SingleTestTime = T.elapsed(); //end timer by logging into variable
		TotalTime += SingleTestTime;
		Results.push_back(SingleTestTime);
		std::this_thread::sleep_for(std::chrono::seconds(IntervalInSeconds)); //add interval to reduce variance in data
	}
	AvgTPerFcn = TotalTime / NumOfTests;

	PrintResults(NumOfTests);
}

//describes the test being carried out
void Performance::PrintDescription(int NumOfTests, long long IntervalInSeconds)
{

	long long total = IntervalInSeconds * NumOfTests;
	std::cout << "TESTS: " << NumOfTests << "\nINTERVAL: "
		<< IntervalInSeconds << "s\n"
		<< "Using 1 core \nComputation Time: " << total << "s\nComputing..." << std::endl;
}

//prints results with user option to review each case
void Performance::PrintResults(int NTests)
{
	std::cout << "\n\nFinished " << NTests << " tests. \nTotal Time: " << TotalTime
		<< "\nAverage Test Time: " << AvgTPerFcn << "\nAll Results(Press 1): \n";
	int answer{ 0 }; std::cin >> answer;
	if (answer == 1)
	{
		int ID{ 0 };
		for (const auto& t : Results)
		{
			++ID;
			std::cout << ID << ": " << t << std::endl;
		}
	}
}