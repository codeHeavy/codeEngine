//UIRadioButton.h
//By Joshua Rand

#pragma once

#include "UIButton.h"

class UIRadioButton;

struct UIRadioGroup {
	UIRadioButton * toggledButton;
};

class UIRadioButton : public UIButton {
private:
	bool toggled;
	XMFLOAT4 toggledColor;
	XMFLOAT4 untoggledColor;
	UIRadioGroup *radioGroup;
public:
	UIRadioButton();
	UIRadioButton(Mesh* mesh, Material* material, float x, float y, float width, float height, XMFLOAT4 color, UIRadioGroup *radioGroup);
	~UIRadioButton();
	void CheckState();
	void Update(float deltaTime, float totalTime);
	void Toggle(bool toggled);
	void OnClick();
};