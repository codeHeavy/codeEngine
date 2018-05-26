#include "DebugWindow.h"

DebugWindow::DebugWindow() {
	timePassed = 0.0f;
	cursorActive = false;
	currentLine = L"";
	enabled = false;
	clearFlag = false;

	for (int i = 0; i < MAX_LINES; i++) {
		lines.push_back(Line{ L"", Colors::White });
	}

	//Add built-in functions for the Debug Window
	CreateFunction(L"Clear", [&](vector<wstring> parameters){ ClearWindow(); });
	CreateFunction(L"ListCommands", [&](vector<wstring> parameters) {ListCommands(); });
	CreateFunction(L"ListParameters", [&](vector<wstring> parameters) {ListParameters(parameters); });
}

DebugWindow::~DebugWindow() {

}

//Clear the debug window history
//Even though this function does not need any parameters, it still needs to match the function signature (return void, take a vector<wstring> parameter)
void DebugWindow::CreateFunction(wstring name, function<void(vector<wstring> parameters)> func) {
	functions.insert(pair<wstring, function<void(vector<wstring> parameters)>>(name, func));
}

bool DebugWindow::FunctionExists(wstring name) {
	function<void(vector<wstring> parameters)> func = functions[name];
	if (func != nullptr) {
		return true;
	}
	else {
		return false;
	}
}

bool DebugWindow::ExecuteFunction(wstring name, vector<wstring> parameters) {
	function<void(vector<wstring> parameters)> func = functions[name];
	if (func != nullptr) {
		func(parameters);
		return true;
	}
	else {
		return false;
	}
}

vector<Line> *DebugWindow::GetLines() {
	return &lines;
}
wstring DebugWindow::GetCurrentLine() {
	return (cursorActive) ? currentLine + L"_" : currentLine;
}

void DebugWindow::Update(float deltaTime) {
	timePassed += deltaTime;
	if (timePassed >= BLINK_INTERVAL) {
		timePassed -= BLINK_INTERVAL;
		cursorActive = !cursorActive;
	}
}

void DebugWindow::AddKey(wstring key) {
	currentLine += key;
	ResetBlinker();
}

//Clear the debug window
//clearFlag indicates whether or not to write the next line on the debug window
//Used only for the Clear command
//Otherwise whenever the user inputs "Clear", they will see an extra "Clear" in the window
void DebugWindow::ClearWindow() {
	clearFlag = true;
	for (int i = 0; i < MAX_LINES; i++) {
		InsertLine(Line{ L"", Colors::White });
	}
}

//List all the available commands with a short description of each
void DebugWindow::ListCommands() {
	InsertLine(Line{ L"BoundingBoxOff - Hide bounding boxes on 3D objects", Colors::LightGreen });
	InsertLine(Line{ L"BoundingBoxOn - Show bounding boxes on 3D objects", Colors::LightGreen });
	InsertLine(Line{ L"WireFrameOn - Show mesh wireframes", Colors::LightGreen });
	InsertLine(Line{ L"WireFrameOff - Show mesh as solid objects", Colors::LightGreen });
	InsertLine(Line{ L"CanvasOff - Turn the UI Canvas off", Colors::LightGreen });
	InsertLine(Line{ L"CanvasOn - Turn the UI Canvas on", Colors::LightGreen });
	InsertLine(Line{ L"Clear - Clear the debug window of previous lines printed", Colors::LightGreen });
	InsertLine(Line{ L"CreateObject - Create an object in 3D space", Colors::LightGreen });
	InsertLine(Line{ L"Exit - Close the application", Colors::LightGreen });
	InsertLine(Line{ L"Print - Write a line to the console window", Colors::LightGreen });
	InsertLine(Line{ L"Quit - Close the application", Colors::LightGreen });
	InsertLine(Line{ L"ThirdPersonCam - Activate 3rd person camera", Colors::LightGreen });
	InsertLine(Line{ L"FreeCam - Activate Free form camera", Colors::LightGreen });
	InsertLine(Line{ L"ChangeSky - Change the skybox", Colors::LightGreen });
}

//List all possible parameters associated with a command, along with a short description
void DebugWindow::ListParameters(vector<wstring> parameters) {
	for (wstring param : parameters) {
		if (param == L"/BoundingBoxOff") {
			InsertLine(Line{ L"BoundingBoxOff - Hide bounding boxes on 3D objects", Colors::LightGreen });
		}
		else if (param == L"/BoundingBoxOn") {
			InsertLine(Line{ L"BoundingBoxOn - Show bounding boxes on 3D objects", Colors::LightGreen });
		}
		if (param == L"/WireFrameOn") {
			InsertLine(Line{ L"WireFrameOn - Show mesh wireframes", Colors::LightGreen });
		}
		else if (param == L"/WireFrameOff") {
			InsertLine(Line{ L"WireFrameOff - Show mesh as solid objects", Colors::LightGreen });
		}
		else if (param == L"/CanvasOff") {
			InsertLine(Line{ L"CanvasOff - Turn the UI Canvas off", Colors::LightGreen });
		}
		else if (param == L"/CanvasOn") {
			InsertLine(Line{ L"CanvasOn - Turn the UI Canvas on", Colors::LightGreen });
		}
		else if (param == L"/Clear") {
			InsertLine(Line{ L"Clear - Clear the debug window of previous lines printed", Colors::LightGreen });
		}
		else if (param == L"/CreateObject") {
			InsertLine(Line{ L"CreateObject - Create an object in 3D space", Colors::LightGreen });
			InsertLine(Line{ L"/shape - Shape of the object (cube, sphere, cone helix, torus)", Colors::LightGreen });
			InsertLine(Line{ L"/x - x position of the object", Colors::LightGreen });
			InsertLine(Line{ L"/y - y position of the object", Colors::LightGreen });
			InsertLine(Line{ L"/z - z position of the object", Colors::LightGreen });
		}
		else if (param == L"/Exit") {
			InsertLine(Line{ L"Exit - Close the application", Colors::LightGreen });
		}
		else if (param == L"/Print") {
			InsertLine(Line{ L"Print - Write a line to the console window", Colors::LightGreen });
			InsertLine(Line{ L"Any text written after \"Print\" will be written to the console window.", Colors::LightGreen });
		}
		else if (param == L"/Quit") {
			InsertLine(Line{ L"Quit - Close the application", Colors::LightGreen });
		}
		else if (param == L"/ThirdPersonCam") {
			InsertLine(Line{ L"ThirdPersonCam - enable the third person camera", Colors::LightGreen });
		}
		else if (param == L"/FreeCam") {
			InsertLine(Line{ L"FreeCam - enable the free form camera", Colors::LightGreen });
		}
		else if (param == L"/ChangeSky") {
			InsertLine(Line{ L"ChangeSky - Change the Skybox", Colors::LightGreen });
		}
	}
}

void DebugWindow::InsertLine(Line line) {
	if (lines.size() >= MAX_LINES) {
		auto it = lines.begin();
		lines.erase(it);
	}
	lines.push_back(line);
}

vector<wstring> DebugWindow::SplitWideString(wstring str) {
	vector<wstring> splitString;
	wstringstream ss(str);
	while (ss) {
		wstring s;
		if (!getline(ss, s, L' '))
			break;
		splitString.push_back(s);
	}
	return splitString;
}

void DebugWindow::EnterPressed() {
	//Insert the current line and move the lines up if the clear flag is not set
	if (!clearFlag) {
		InsertLine(Line{ currentLine, Colors::White });
	}
	clearFlag = false;

	//Get parameter list
	//Set the first string in the line as the function name, the rest are parameters
	vector<wstring> params;
	wstring functionName = L"";
	if (currentLine.length() > 0) {
		params = SplitWideString(currentLine);
		functionName = params[0];
		params.erase(params.begin());
	}

	//Execute the function. Result returns true if the function is valid, false otherwise
	//The first value in params is the function name, the rest are parameters
	bool functionExists = ExecuteFunction(functionName, params);

	//Raise an error message if the command is not recognized
	if (!functionExists){// && currentLine.length() > 0) {
		wstring errorMsg = L"Error. Unrecognized command: " + currentLine;
		InsertLine(Line{ errorMsg, Colors::Red });
	}
	currentLine = L"";
}

//Remove the very last character from the string when backspace is pressed
void DebugWindow::BackspacePressed() {
	if (currentLine.size() > 0)
		currentLine = currentLine.erase(currentLine.size() - 1);
	ResetBlinker();
}

//Turn on or off the debug window when the Grave Key is pressed
void DebugWindow::GravePressed() {
	enabled = !enabled;
}

//Returns whether or not the DebugWindiw is active
bool DebugWindow::IsEnabled() {
	return enabled;
}

//Reset the time on the blinker whenever pressing a key
//The underscore _ blinks on and off every half second
void DebugWindow::ResetBlinker() {
	timePassed = 0.0f;
	cursorActive = true;
}