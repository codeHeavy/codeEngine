//------------------------------------
// By Jishnu Girish
//------------------------------------
#include "Shader.h"

//---------------------------------------------------
// Vertex shader definitions
//---------------------------------------------------
VertexShader::VertexShader(const std::wstring& filePath, ID3D11Device* device,ID3D11DeviceContext* context)
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(filePath.c_str());
};

VertexShader::~VertexShader() { 
	//delete vertexShader; 
};


//---------------------------------------------------
// Pixel shader definitions
//---------------------------------------------------
PixelShader::PixelShader(const std::wstring& filePath, ID3D11Device* device, ID3D11DeviceContext* context)
{
	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(filePath.c_str());
};

PixelShader::~PixelShader() { 
	//delete pixelShader; 
};