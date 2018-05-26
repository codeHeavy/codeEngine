#pragma once
#include <DirectXMath.h>

using namespace DirectX;
struct DirectionalLight 
{
	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
	XMFLOAT3 direction;
};

struct PointLight
{
	XMFLOAT4 color;
	XMFLOAT3 position;
};