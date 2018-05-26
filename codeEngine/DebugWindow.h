#pragma once

#include <map>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <DirectXMath.h>
#include <DirectXColors.h>

using namespace DirectX;
using namespace DirectX;
using namespace std;

const float BLINK_INTERVAL = 0.5f;
const int MAX_LINES = 15;

struct Line {
	wstring text;
	XMVECTOR color;
};

class DebugWindow {
public:
	DebugWindow();
	~DebugWindow();
	void CreateFunction(wstring name, function<void(vector<wstring> parameters)> func);
	vector<Line> *GetLines();
	wstring GetCurrentLine();
	void Update(float deltaTime);
	void AddKey(wstring key);
	void EnterPressed();
	void BackspacePressed();
	void GravePressed();
	bool IsEnabled();
private:
	bool enabled;
	vector<Line> lines;
	map<wstring, function<void(vector<wstring> parameters)>> functions;
	wstring currentLine;
	float timePassed;
	bool cursorActive;
	void ClearWindow();
	void ListCommands();
	void ListParameters(vector<wstring> parameters);
	void InsertLine(Line line);
	vector<wstring> SplitWideString(wstring str);
	bool FunctionExists(wstring name);
	bool ExecuteFunction(wstring name, vector<wstring> parameters);
	void ResetBlinker();
	bool clearFlag;
};