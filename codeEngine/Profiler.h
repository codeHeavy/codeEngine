//-------------------------------------------------------------------------------
// The implementation of the profiler was inspired by the forum discussion 
// in the following website
// https://arstechnica.com/civis/viewtopic.php?f=20&t=262094
//-------------------------------------------------------------------------------
#pragma once
#include <Windows.h>
#include <vector>
#include <set>
#include <map>
#include <ostream>
#include <fstream>
#include <algorithm>

typedef __int64 int64;
#define PROFILE ProfilerStart NewProfile(__FILE__, __FUNCTION__, __LINE__);

struct PerformanceData 
{
	std::string fileName;
	std::string functionName;
	int lineNumber;
	int64 cycles;
	int64 functionCalls;

	PerformanceData(std::string _fName, std::string _funcName, int lineNum, int _cycles = 0, int _funcCalls = 0)
		: fileName(_fName), functionName(_funcName), lineNumber(lineNum), cycles(_cycles), functionCalls(_funcCalls) {};
};

typedef std::vector<PerformanceData*> DataStack;
typedef std::vector<PerformanceData> DataLog;

class Profiler
{
private:
	static Profiler* instance;
	Profiler();

	DataStack profilerStack;
	DataLog profilerLog;

	LARGE_INTEGER startTime;
	double frequency;

	int64 totalTime;
	int64 lastTime;
public:
	static Profiler * GetInstance();
	static void DeleteInstance(int64 deleteTime);
	void Start(std::string _fName, std::string _funcName, int lineNum);
	void End();
	LARGE_INTEGER GetClock();
	void LogPerformance(int64 globalTime);
	~Profiler();
	bool CheckLog(std::string funcName);
	void Sort();
};

class ProfilerStart
{
public:
	ProfilerStart(std::string fileName, std::string functionName, int lineNumber);
	~ProfilerStart();
	void endMark();
private:
	bool onStack;
};