//UIButton.cpp
//By Joshua Rand

#include "UIButton.h"

UIButton::UIButton() : UIObject() {
	state = ButtonState::Normal;
}
UIButton::UIButton(Mesh* mesh, Material* material, float x, float y, float width, float height, wstring text, XMFLOAT4 color) : UIObject(mesh, material, x, y, width, height, color) {
	//hoverColor = XMFLOAT4(color.x + 0.5f, color.y + 0.5f, color.z + 0.5f, color.w);
	//pressedColor = XMFLOAT4(color.x - 0.5f, color.y - 0.5f, color.z - 0.5f, color.w);
	normalColor = color;
	hoverColor = normalColor * Colors::LightGray;
	pressedColor = normalColor * Colors::Gray;
	state = ButtonState::Normal;
	this->text = text;
}
UIButton::~UIButton() {

}

wstring UIButton::GetText() {
	return this->text;
}

void UIButton::CheckState() {
	switch (state) {
	case ButtonState::Normal:
		currentColor = color;
		break;

	case ButtonState::Hover:
		currentColor = hoverColor;
		break;

	case ButtonState::Pressed:
		currentColor = pressedColor;
		break;
	}
}

void UIButton::Update(float deltaTime, float totalTime) {
	UIObject::Update(deltaTime, totalTime);
	CheckState();
}

Color UIButton::GetColor() {
	return currentColor;
}

void UIButton::OnMouseMove(float mouseX, float mouseY) {
	float left = x - (width / 2.0f);
	float right = x + (width / 2.0f);
	float top = y + (height / 2.0f);
	float bottom = y - (height / 2.0f);

	if (mouseX >= left && mouseX <= right && mouseY >= bottom && mouseY <= top) {
		mouseOver = true;
		if (state == ButtonState::Normal && !mousePressed) {
			state = ButtonState::Hover;
		}
	}
	else {
		mouseOver = false;
		if (state == ButtonState::Hover) {
			state = ButtonState::Normal;
		}
	}
}

void UIButton::OnMouseDown(float mouseX, float mouseY) {
	mousePressed = true;
	if (mouseOver && mousePressed && state == ButtonState::Hover) {
		state = ButtonState::Pressed;
	}
}

void UIButton::OnMouseUp() {
	if (state == ButtonState::Pressed && mouseOver) {
		OnClick();
	}
	mousePressed = false;
	state = ButtonState::Normal;
}

void UIButton::AssignDelegateFunction(void *function) {
	void *del = function;
	delegateFunctions.push_back(del);
}

void UIButton::OnClick() {
	/*for (auto delegateFunction : delegateFunctions) {
		void(*func)() = (void(*)())delegateFunction;
		func();
	}*/
	if(myfunction != nullptr)
		myfunction();
}

void UIButton::AssignFunc(std::function<void()> myfunc)
{
	myfunction = myfunc;
}