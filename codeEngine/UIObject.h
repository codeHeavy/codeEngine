//UIObject.h
//By Joshua Rand

#pragma once

#include "GameObject.h"
#include "SimpleMath.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;

class UIObject : public GameObject {
protected:
	Color color;
	Color currentColor;
	float x;
	float y;
	float width;
	float height;
	int pixelWidth;
	int pixelHeight;
	bool mouseOver;
	bool mousePressed;
	void CalculateWidthAndHeight();
	bool calculated;
public:
	UIObject();
	UIObject(Mesh* mesh, Material* material, float x, float y, float width, float height, XMFLOAT4 color);
	virtual ~UIObject();
	void SetColor(XMFLOAT4 color);
	void SetMaterial(Material *m);
	virtual Color GetColor();
	float GetX();
	float GetY();
	Vector2 GetScale();
	Vector2 GetPixelScale();
	virtual void Update(float deltaTime, float totalTime);
	virtual void OnMouseMove(float x, float y);
	virtual void OnMouseDown(float x, float y);
	virtual void OnMouseUp();
};