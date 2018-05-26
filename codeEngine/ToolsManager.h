//---------------------------------
// By Jishnu Girish
//---------------------------------

#pragma once
#include <Windows.h>
#include <vector>
#include "Tool.h"
#include <memory>
#include <iostream>
class ToolsManager
{
private:
	std::vector<std::shared_ptr<Tool>> registeredTools;
	ToolsManager();
public:
	ToolsManager(ToolsManager const&) = delete;
	void operator=(ToolsManager const&) = delete;
	static ToolsManager& Instance()
	{
		static ToolsManager instance;
		return instance;
	}
	void RegisterTool(LPCTSTR fullPath);
	void StartTool(LPCSTR toolName);
	void CloseTool(LPCSTR fullPath);
	void UnRegisterTool(LPCTSTR toolName);
	void UnregisterAll();
	std::shared_ptr<Tool> GetTool(LPCSTR toolName);
	~ToolsManager();
};

