//UIRadioButton.cpp
//By Joshua Rand

#include "UIRadioButton.h"

UIRadioButton::UIRadioButton() : UIButton() {
	toggled = false;
	toggledColor = pressedColor;
	untoggledColor = color;
}
UIRadioButton::UIRadioButton(Mesh* mesh, Material* material, float x, float y, float width, float height, XMFLOAT4 color, UIRadioGroup *radioGroup) : UIButton(mesh, material, x, y, width, height, L"", color) {
	//hoverColor = XMFLOAT4(color.x + 0.5f, color.y + 0.5f, color.z + 0.5f, color.w);
	//pressedColor = XMFLOAT4(color.x - 0.5f, color.y - 0.5f, color.z - 0.5f, color.w);
	//state = ButtonState::Normal;
	toggled = false;
	toggledColor = pressedColor;
	untoggledColor = color;
	this->radioGroup = radioGroup;
	this->text = L"";
}
UIRadioButton::~UIRadioButton() {

}

void UIRadioButton::CheckState() {
	switch (state) {
	case ButtonState::Normal:
		currentColor = (toggled) ? toggledColor : untoggledColor;
		break;

	case ButtonState::Hover:
		currentColor = hoverColor;
		break;

	case ButtonState::Pressed:
		currentColor = pressedColor;
		break;
	}
}

void UIRadioButton::Update(float deltaTime, float totalTime) {
	UIButton::Update(deltaTime, totalTime);
}

void UIRadioButton::Toggle(bool toggled) {
	this->toggled = toggled;
	//Check if no buttons have been toggled yet and make sure the currently toggled button is not this one
	if (radioGroup->toggledButton != nullptr && radioGroup->toggledButton != this)
		radioGroup->toggledButton->toggled = false;
	radioGroup->toggledButton = this;
}

void UIRadioButton::OnClick() {
	UIButton::OnClick();
	Toggle(true);
}