#pragma once

#include "SimpleShader.h"
class VertexShader
{
private:
	SimpleVertexShader *vertexShader;
public:
	VertexShader(const std::wstring& filePath,ID3D11Device *device, ID3D11DeviceContext *context);
	~VertexShader();
	SimpleVertexShader* Get() const { return vertexShader; };
};

class PixelShader
{
private:
	SimplePixelShader *pixelShader;
public:
	PixelShader(const std::wstring& filePath, ID3D11Device* device, ID3D11DeviceContext* context);
	~PixelShader();
	SimplePixelShader* Get() const { return pixelShader; };
};

