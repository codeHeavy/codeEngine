//DialogueSystem.h
//By Joshua Rand

#pragma once

#include <CommonStates.h>
#include <d3d11.h>
#include <fstream>
#include "Portrait.h"
#include "ResourceManager.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include <string>
#include <vector>

const float CHAR_RATE = 30.0f;
const int XOFFSET = 100;
const int YOFFSET = 0;

using namespace DirectX;
using namespace std;
using namespace SimpleMath;

class DialogueSystem {
public:
	DialogueSystem(ID3D11Device* device, ID3D11DeviceContext* context, ResourceManager *rm, unsigned int width, unsigned int height);
	~DialogueSystem();
	void LoadLinesFromFile(string filename);
	void Update(float deltaTime);
	void DrawPortraits();
	void Draw();
	void NextButton();
	void SkipButton();
	void UpdateWidthAndHeight(unsigned int width, unsigned int height);
	bool Active();
private:
	ID3D11Device *device;
	ID3D11DeviceContext *context;
	unique_ptr<SpriteBatch> spriteBatch;
	unique_ptr<SpriteFont> spriteFont;
	vector<string> lines;

	ResourceManager *rm;
	//ID3D11ShaderResourceView *leftPortrait;
	//ID3D11ShaderResourceView *rightPortrait;
	Portrait *leftPortrait;
	Portrait *rightPortrait;
	string leftPortraitFilename;
	string rightPortraitFilename;

	void LoadPortraits();

	float currentChars;
	float maxChars;
	int currentLineIndex;
	string text;
	bool started;
	bool endOfLine;

	unsigned int width;
	unsigned int height;
};