//---------------------------------
// By Jishnu Girish
//---------------------------------
#include "ToolsManager.h"

//--------------------------------------------
// Callback for process creation
//--------------------------------------------
int CALLBACK EnumWindowsProc(HWND hwnd, LPARAM param)
{
	DWORD pID;
	DWORD TpID = GetWindowThreadProcessId(hwnd, &pID);
	if (TpID == (DWORD)param)
	{
		//apphwnd = hwnd;
		return false;
	}
	return true;
}

ToolsManager::ToolsManager()
{
}


ToolsManager::~ToolsManager()
{
}

//-----------------------------------------------------------
// Add tools to the engine registry
// All tools have to be executable files
//-----------------------------------------------------------
void ToolsManager::RegisterTool(LPCTSTR fullPath)
{
	try {
		// Check if the tool is already registered
		for (auto tool : registeredTools)
		{
			if (strcmp(tool->GetPath(), fullPath) == 0)
				return;
		}
		std::shared_ptr<Tool> newTool = std::make_shared<Tool>();	// Create new reference to tool
		newTool->SetPath(fullPath);

		std::string name = (std::string)fullPath;
		auto a = name.find_last_of("\\");							// Extract name (only supports "\")
		name = name.substr(a + 1, name.size());
		newTool->SetName(name);

		registeredTools.emplace_back(newTool);						// Register new tool
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

//-----------------------------------------------------------
// Create a new process and primary thread for a tool and executes it
// All tools have to be executable files
//-----------------------------------------------------------
void ToolsManager::StartTool(LPCSTR _toolName)
{
	try {
		for (auto tool : registeredTools)
		{
			if (std::string(tool->GetName()).compare(std::string(_toolName)) != 0)
				continue;

			CreateProcess(tool->GetPath(),			// The path
				0,									// Command line
				NULL,								// Process handle not inheritable
				NULL,								// Thread handle not inheritable
				FALSE,								// Set handle inheritance to FALSE
				0,									// No creation flags
				NULL,								// Use parent's environment block
				NULL,								// Use parent's starting directory 
				&tool->GetStartInfo(),				// Pointer to STARTUPINFO structure
				&tool->GetProcessInfo()             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
			);
			::EnumWindows(&EnumWindowsProc, tool->GetProcessInfo().dwThreadId);		//	Callback for checking if process created sucessfully
			::PostThreadMessage(tool->GetProcessInfo().dwThreadId, WM_CLOSE, 0, 0); //	Post messages
			tool->SetStatus(true);					// Set tool as running
			return;
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

//------------------------------------------------------
// Terminate the process and thread the tool is running on
// Close the handles to the process and thread
//------------------------------------------------------
void ToolsManager::CloseTool(LPCSTR _fullPath)
{
	try {
		for (auto tool : registeredTools)
		{
			if (std::string(tool->GetPath()).compare(std::string(_fullPath)) != 0)
				continue;
			TerminateProcess(tool->GetProcessInfo().hProcess, NULL);
			TerminateThread(tool->GetProcessInfo().hThread, NULL);
			CloseHandle(tool->GetProcessInfo().hProcess);
			CloseHandle(tool->GetProcessInfo().hThread);
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

//-----------------------------------------------------------
// Unregister a tool from the list of registered tools
//-----------------------------------------------------------
void ToolsManager::UnRegisterTool(LPCTSTR _toolName)
{
	try {
		auto it = registeredTools.begin() - 1;
		for (auto tool : registeredTools)
		{
			it++;
			if (std::string(tool->GetName()).compare(std::string(_toolName)) != 0)
				continue;
			registeredTools.erase(it);
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

//-----------------------------------------------------------
// Unregister all tools from the list of registered tools
//-----------------------------------------------------------
void ToolsManager::UnregisterAll()
{
	try {
		for (auto tool : registeredTools)
		{
			TerminateProcess(tool->GetProcessInfo().hProcess, NULL);
			TerminateThread(tool->GetProcessInfo().hThread, NULL);
			CloseHandle(tool->GetProcessInfo().hProcess);
			CloseHandle(tool->GetProcessInfo().hThread);
		}
		registeredTools.clear();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

//-----------------------------------------------------------
// Return instance of a registered tool
//-----------------------------------------------------------
std::shared_ptr<Tool> ToolsManager::GetTool(LPCSTR _toolName)
{
	try 
	{
		for (auto tool : registeredTools)
		{
			if (std::string(tool->GetName()).compare(std::string(_toolName)) != 0)
				continue;
			return tool;
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}