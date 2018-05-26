//---------------------------------------------------------
// By Jishnu Girish
//---------------------------------------------------------
#include "Profiler.h"
Profiler* Profiler::instance = 0;
Profiler::Profiler() { lastTime = GetTickCount(); }
Profiler* Profiler::GetInstance() {
	if (!instance)
		instance = new Profiler();
	return instance;
}

//---------------------------------------------------------------
// Delete profiler instance
//---------------------------------------------------------------
void Profiler::DeleteInstance(int64 globalTotal)
{
	Profiler::GetInstance()->LogPerformance(globalTotal);
	delete Profiler::GetInstance();
}

//---------------------------------------------------------------
// Release all allocated memory
//---------------------------------------------------------------
Profiler::~Profiler()
{
#ifdef CE_DEBUG
	for (auto a : profilerStack)
	{
		delete a;
	}
#endif
}

//---------------------------------------------------------------
// Return the high performance counter
//---------------------------------------------------------------
LARGE_INTEGER Profiler::GetClock()
{
	LARGE_INTEGER clockCycles;
	QueryPerformanceCounter(&clockCycles);
	return clockCycles;
}

//---------------------------------------------------------------
// Start tracking a functions performance data
//---------------------------------------------------------------
void Profiler::Start(std::string _fName, std::string _funcName, int lineNum)
{
	int64 startTime = GetTickCount();

	// Check the call stack
	if (profilerStack.size())
	{
		auto lastMarker = profilerStack.rbegin();
		__int64 runTime = (startTime - lastTime);	//Update the total time spent processing the previous function in stack
		(*lastMarker)->cycles += runTime;
		totalTime += runTime;
	}

	// Maintain only one entry per function in the log
	if (!CheckLog(_funcName))
	{
		profilerLog.push_back(PerformanceData(_fName, _funcName, lineNum));
		profilerLog.back().functionCalls++;
		profilerStack.push_back(&profilerLog.back());
	}
	else
	{
		for (auto &a : profilerLog)
		{
			if (a.functionName == _funcName)
			{
				a.functionCalls++;
				profilerStack.push_back(&a);
			}
		}
	}
	lastTime = GetTickCount();
}

//---------------------------------------------------------------
// Stop tracking a function
//---------------------------------------------------------------
void Profiler::End()
{
	int64 exitTime = GetTickCount();
	int64 runTime = exitTime - lastTime;

	auto lastMarker = profilerStack.rbegin();
	(*lastMarker)->cycles += runTime;
	totalTime += runTime;
	profilerStack.pop_back();
	lastTime = GetTickCount();
}

//---------------------------------------------------------------
// Log the performance data of a function
//---------------------------------------------------------------
void Profiler::LogPerformance(int64 globalTotal)
{
	std::ofstream os;
#ifdef CE_DEBUG
	os.open("../../log.txt");
#else 
	os.open("log.txt");
#endif
	Sort();
	for (auto i : profilerLog)
	{
		if (i.lineNumber)

		{   //Truncate path from file name
			auto a = i.fileName.find_last_of("\\");
			auto fName = i.fileName.substr(a + 1, i.fileName.size());
			a = fName.find_last_of("/");
			fName = fName.substr(a + 1, i.fileName.size());

			os << "[";
			os.width(12);
			os << (double)i.cycles/ 1000.0f << "]";
			os << " milliseconds (";
			os.width(12);
			double percent = double(i.cycles) / double(globalTotal) * 100;		// Calculate total cpu percentage spent in the function
			percent = std::abs(percent);
			os << percent << "%) used in [";
			os.width(8);
			os << i.functionCalls << "] calls by " << fName.c_str() << " " << i.functionName.c_str()<< " at line " << i.lineNumber << std::endl;
		}

	}
	os.close();
}

//---------------------------------------------------------------
// Start tracking a function
//---------------------------------------------------------------
ProfilerStart::ProfilerStart(std::string fileName, std::string functionName, int lineNumber)
{
	Profiler::GetInstance()->Start(fileName, functionName, lineNumber);
}

//---------------------------------------------------------------
// Remove a function from the stack and pause tracking
//---------------------------------------------------------------
ProfilerStart::~ProfilerStart()
{
	endMark();
}

//---------------------------------------------------------------
// Remove a function from the stack and pause tracking
//---------------------------------------------------------------
void ProfilerStart::endMark()
{
	if (onStack)
	{
		Profiler::GetInstance()->End();
		onStack = false;
	}
}

//---------------------------------------------------------------
// Return true if a function exists in the log
//---------------------------------------------------------------
bool Profiler::CheckLog(std::string funcName)
{
	for (auto &a : profilerLog)
	{
		if (a.functionName == funcName)
			return true;
	}
	return false;
}

//---------------------------------------------------------------
// Sort the log by cycles
//---------------------------------------------------------------
void Profiler::Sort()
{
	std::sort(profilerLog.begin(), profilerLog.end(),
		[&](PerformanceData a, PerformanceData b) { return (a.cycles > b.cycles); });
}