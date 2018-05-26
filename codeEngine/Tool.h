//---------------------------------
// By Jishnu Girish
//---------------------------------
#pragma once
#include <Windows.h>
#include <string>
class Tool
{
private:
	std::string applicationName;
	LPCTSTR path;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	bool running;
public:
	Tool();
	~Tool();
	void SetPath(LPCTSTR _path);
	void SetName(std::string _name);
	void SetStartInfor(STARTUPINFO _si);
	void SetProcessInfo(PROCESS_INFORMATION _pi);
	void SetStatus(bool status);

	bool isRunning();
	LPCTSTR GetPath();
	std::string GetName();
	STARTUPINFO GetStartInfo();
	PROCESS_INFORMATION GetProcessInfo();
};

