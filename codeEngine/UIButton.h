//UIButton.h
//By Joshua Rand

#pragma once

#include "UIObject.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <string>
#include <vector>
#include <functional>

using namespace DirectX;
using namespace std;

enum ButtonState {
	Normal = 0,
	Hover,
	Pressed
};

class UIButton : public UIObject {
protected:
	//XMFLOAT4 hoverColor;
	//XMFLOAT4 pressedColor;
	Color normalColor;
	Color hoverColor;
	Color pressedColor;
	Color currentColor;
	ButtonState state;
	wstring text;
	vector<void*> delegateFunctions;
	std::function<void()> myfunction;
public:
	UIButton();
	UIButton(Mesh* mesh, Material* material, float x, float y, float width, float height, wstring text, XMFLOAT4 color);
	virtual ~UIButton();
	virtual void CheckState();
	virtual void Update(float deltaTime, float totalTime);
	virtual Color GetColor();
	void OnMouseMove(float mouseX, float mouseY);
	void OnMouseDown(float mouseX, float mouseY);
	void OnMouseUp();
	void AssignDelegateFunction(void *function);
	void AssignFunc(std::function<void()> myfunc);
	virtual void OnClick();
	wstring GetText();
};