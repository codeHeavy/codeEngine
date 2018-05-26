//---------------------------------
// By Jishnu Girish
//---------------------------------
#include "Tool.h"

//---------------------------------------------------------
// Initialise process information
//---------------------------------------------------------
Tool::Tool()
{
	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	running = false;
}

Tool::~Tool()
{
}

//---------------------------------------------------------
// Set path/directory of the tool
//---------------------------------------------------------
void Tool::SetPath(LPCTSTR _path)
{
	path = _path;
}

//---------------------------------------------------------
// Return the path of the tool
//---------------------------------------------------------
LPCTSTR Tool::GetPath()
{
	return path;
}

//---------------------------------------------------------
// Set the name of the tool
//---------------------------------------------------------
void Tool::SetName(std::string _name)
{
	applicationName = _name;
}

//---------------------------------------------------------
// Return the name of the tool
//---------------------------------------------------------
std::string Tool::GetName()
{
	return applicationName;
}

//---------------------------------------------------------
// Set process start information
//---------------------------------------------------------
void Tool::SetStartInfor(STARTUPINFO _si)
{
	si = _si;
}

//---------------------------------------------------------
// Set process information
//---------------------------------------------------------
void Tool::SetProcessInfo(PROCESS_INFORMATION _pi)
{
	pi = _pi;
}

//---------------------------------------------------------
// Return start information
//---------------------------------------------------------
STARTUPINFO Tool::GetStartInfo()
{
	return si;
}

//---------------------------------------------------------
// Return process information
//---------------------------------------------------------
PROCESS_INFORMATION Tool::GetProcessInfo()
{
	return pi;
}

//---------------------------------------------------------
// Check if the tool is running
//---------------------------------------------------------
bool Tool::isRunning()
{
	return running;
}

//---------------------------------------------------------
// Set the runnning status of the tool
//---------------------------------------------------------
void Tool::SetStatus(bool status)
{
	running = status;
}