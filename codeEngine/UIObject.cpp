//UIObject.cpp
//By Joshua Rand

#include "UIObject.h"

UIObject::UIObject() : GameObject() {
	
}
UIObject::UIObject(Mesh* mesh, Material* material, float x, float y, float width, float height, XMFLOAT4 color) : GameObject(mesh, material) {
	this->color = color;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	currentColor = color;
	mouseOver = false;
	mousePressed = false;
	calculated = false;
}
UIObject::~UIObject() {

}

void UIObject::SetColor(XMFLOAT4 color) {
	this->color = color;
}

//Memory management should be handled externally
void UIObject::SetMaterial(Material *m) {
	this->material = m;
}

//Should probably make this some utility function
void UIObject::CalculateWidthAndHeight() {
	ID3D11Resource * rs = nullptr;
	D3D11_TEXTURE2D_DESC desc;

	material->GetSRV()->GetResource(&rs);
	ID3D11Texture2D *texture = (ID3D11Texture2D*)(rs);
	texture->GetDesc(&desc);

	pixelWidth = desc.Width;
	pixelHeight = desc.Height;
	texture->Release();
}

Color UIObject::GetColor()
{
	return currentColor;
}

float UIObject::GetX() {
	return x;
}

float UIObject::GetY() {
	return y;
}

Vector2 UIObject::GetScale() {
	return Vector2(width, height);
}

Vector2 UIObject::GetPixelScale() {
	return Vector2(pixelWidth, pixelHeight);
}

void UIObject::Update(float deltaTime, float totalTime) {
	if (!calculated && this->material->GetSRV() != nullptr) {
		CalculateWidthAndHeight();
		calculated = true;
	}
}

void UIObject::OnMouseMove(float x, float y) {

}

void UIObject::OnMouseDown(float x, float y) {

}

void UIObject::OnMouseUp() {

}