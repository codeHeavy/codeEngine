//UIObjectLoader.h
//By Joshua Rand

#pragma once

#include <algorithm>
#include <fstream>
#include <map>
#include "Material.h"
#include "ResourceManager.h"
#include <string>
#include <sstream>
#include <vector>
#include "UICanvas.h"
using namespace std;

enum UIObjectType {
	Button = 0,
	RadioButton,
	Text,
	Sprite
};

struct UIObjectInfo {
	UIObjectType objType;
	float x;
	float y;
	float width;
	float height;
	float r;
	float g;
	float b;
	string text;
	Alignment alignment;
	int radioGroup;
	string filename;
};

class UIObjectLoader {
public:
	UIObjectLoader();
	~UIObjectLoader();
	void LoadObjectsFromFile(string filename, ResourceManager *rm, UICanvas &canvas, Material *material);
private:
	void CreateObjects(vector<UIObjectInfo> info, ResourceManager *rm, UICanvas &canvas, Material *material);
	//Keep track of materials, to be deleted afterward
	//In the future, may be reasonable to store in a map to avoid creating multiple materials with the same texture
	vector<Material*> materials;
};