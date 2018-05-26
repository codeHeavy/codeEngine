//UIObjectLoader.cpp
//By Joshua Rand

#include "UICanvas.h"
#include "UIObjectLoader.h"

UIObjectLoader::UIObjectLoader() {

}

//Load each line from the file
void UIObjectLoader::LoadObjectsFromFile(string filename, ResourceManager *rm, UICanvas &canvas, Material *material) {
	//Load each line into a vector of strings
	vector<string> lines;
	ifstream infile(filename);
	if (!infile.good())
		return;
	while (infile) {
		string line;
		if (!getline(infile, line)) {
			break;
		}
		lines.push_back(line);
	}

	vector<wstring> wlines;

	//Parse each line and store values into a struct
	vector<UIObjectInfo> allObjectInfo;
	for (auto line : lines) {
		vector<string> splitString;
		istringstream ss(line);
		while (ss) {
			string s;
			if (!getline(ss, s, ','))
				break;
			splitString.push_back(s);
		}
		
		//File read from UIObjects.txt
		//File format determined by splitString[0] (Button/RadioButton/TextEntity/Sprite
		//Button:		Button,x position,y position,width,height,red,green,blue,text
		//RadioButton:	RadioButton,x position,y position,width,height,red,green,blue,radio group ID (same ID for radio buttons in the same group)
		//TextEntity:	TextEntity,text,alignment (left/right/center),x position,y position
		//Sprite:		Sprite,filename,x position,y position,width,height,red,green,blue
		UIObjectInfo info;
		if (splitString[0] == "Button" || splitString[0] == "RadioButton") {
			info.objType = (splitString[0] == "Button") ? UIObjectType::Button : UIObjectType::RadioButton;
			info.x = stof(splitString[1]);
			info.y = stof(splitString[2]);
			info.width = stof(splitString[3]);
			info.height = stof(splitString[4]);
			info.r = stof(splitString[5]);
			info.g = stof(splitString[6]);
			info.b = stof(splitString[7]);
			if (splitString[0] == "Button")
				info.text = splitString[8];
			if (splitString[0] == "RadioButton")
				info.radioGroup = stoi(splitString[8]);
			info.filename = splitString[9];
		}
		else if (splitString[0] == "TextEntity") {
			Alignment align = Alignment::Left;
			if (splitString[2] == "Left")
				align = Alignment::Left;
			else if (splitString[2] == "Right")
				align = Alignment::Right;
			else if (splitString[2] == "Center")
				align = Alignment::Center;
			else {
				return;
			}

			info.objType = UIObjectType::Text;
			info.text = splitString[1];
			info.alignment = align;
			info.x = stof(splitString[3]);
			info.y = stof(splitString[4]);
		}
		else if (splitString[0] == "Sprite") {
			info.objType = UIObjectType::Sprite;
			info.filename = splitString[1];
			info.x = stof(splitString[2]);
			info.y = stof(splitString[3]);

			info.width = stof(splitString[4]);
			info.height = stof(splitString[5]);

			info.r = stof(splitString[6]);
			info.g = stof(splitString[7]);
			info.b = stof(splitString[8]);
		}
		else {
			return;
		}
		allObjectInfo.push_back(info);
	}

	CreateObjects(allObjectInfo, rm, canvas, material);
}

UIObjectLoader::~UIObjectLoader() {
	for (auto mat : materials) {
		delete mat;
	}
}

void UIObjectLoader::CreateObjects(vector<UIObjectInfo> allInfo, ResourceManager *rm, UICanvas &canvas, Material *buttonMaterial) {
	wstring str;
	UIRadioGroup *getRadioGroup;
	wstring filename;
	Material *m;
	for (auto info : allInfo) {
		switch (info.objType) {
		case UIObjectType::Button:
			filename = wstring(info.filename.begin(), info.filename.end());
			m = new Material(rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"), rm->GetTextureByName(filename));
			materials.push_back(m);
			str = wstring(info.text.begin(), info.text.end());
			canvas.CreateButton(info.x, info.y, info.width, info.height, m, str.c_str(), XMFLOAT4(info.r, info.g, info.b, 1.0f));
			break;

		case UIObjectType::RadioButton:
			//Create radio buttons and create a radio group for them if one does not already exist
			getRadioGroup = canvas.CreateRadioGroup(info.radioGroup);
			filename = wstring(info.filename.begin(), info.filename.end());
			m = new Material(rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"), rm->GetTextureByName(filename));
			materials.push_back(m);
			canvas.CreateRadioButton(info.x, info.y, info.width, info.height, XMFLOAT4(info.r, info.g, info.b, 1.0f), getRadioGroup, m);
			break;

		case UIObjectType::Text:
			str = wstring(info.text.begin(), info.text.end());
			canvas.CreateText(str.c_str(), info.alignment, info.x, info.y);
			break;

		case UIObjectType::Sprite:
			filename = wstring(info.filename.begin(), info.filename.end());
			m = new Material(rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"), rm->GetTextureByName(filename));
			materials.push_back(m);
			canvas.CreateObject(info.x, info.y, info.width, info.height, m, XMFLOAT4(info.r, info.g, info.b, 1.0f));
			break;
		}
	}
}