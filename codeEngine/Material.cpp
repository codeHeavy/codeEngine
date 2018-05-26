//---------------------------------------------------------
// By Jishnu Girish
//---------------------------------------------------------
#include "Material.h"

Material::Material()
{
	vertexShader = nullptr;
	pixelShader = nullptr;
	srv = nullptr;
	sampler = nullptr;
}

//-------------------------------------------------------------------
// Initialize a simple matirial with just shader data
//-------------------------------------------------------------------
Material::Material(SimpleVertexShader* vs, SimplePixelShader* ps)
{
	vertexShader = vs;
	pixelShader = ps;
	srv = nullptr;
	sampler = nullptr;
}

//-------------------------------------------------------------------
// Initialize a matirial with textures and shaders
//-------------------------------------------------------------------
Material::Material(SimpleVertexShader* vs, SimplePixelShader* ps, ID3D11ShaderResourceView* _srv, ID3D11SamplerState* _sampler)
{
	vertexShader = vs;
	pixelShader = ps;
	srv = _srv;
	sampler = _sampler;
	texture = nullptr;
	normalTexture = nullptr;
	normalsrv = nullptr;
}

//-------------------------------------------------------------------
// Initialize a matirial with textures and shaders
//-------------------------------------------------------------------
Material::Material(SimpleVertexShader* vs, SimplePixelShader* ps, Texture* _texture)
{
	vertexShader = vs;
	pixelShader = ps;
	texture = _texture;
	srv = _texture->GetSRV();
	sampler = _texture->GetSampler();
	normalTexture = nullptr;
	normalsrv = nullptr;
}

//-------------------------------------------------------------------
// Initialize a matirial with textures and shaders
//-------------------------------------------------------------------
Material::Material(SimpleVertexShader* vs, SimplePixelShader* ps, Texture* _texture, Texture* _normalTexture)
{
	vertexShader = vs;
	pixelShader = ps;
	texture = _texture;
	normalTexture = _normalTexture;
	srv = _texture->GetSRV();
	normalsrv = _normalTexture->GetSRV();
	sampler = _texture->GetSampler();
}

//-------------------------------------------------------------------
// Release all created data
//-------------------------------------------------------------------
Material::~Material()
{
	//-----------------------------------------------
	// Resource manager is handling the release of
	// allocated memory
	//-----------------------------------------------
	
	//if (vertexShader)
		//delete vertexShader;
	//if (pixelShader)
		//delete pixelShader;
	//if (srv) { srv->Release(); }
	//if (sampler) { sampler->Release(); }
	//if (texture) { delete texture; }
}

SimplePixelShader * Material::GetPixelShader()
{
	return pixelShader;
}

SimpleVertexShader * Material::GetVertexShader()
{
	return vertexShader;
}

ID3D11ShaderResourceView* Material::GetSRV()
{
	return srv;
}

ID3D11ShaderResourceView* Material::GetNormalSRV()
{
	return normalsrv;
}

ID3D11SamplerState* Material::GetSampler()
{
	return sampler;
}

//-------------------------------------------------------------------
// Load textures into the material
//-------------------------------------------------------------------
void Material::LoadTexture(const wchar_t* filePath, ID3D11Device* device, ID3D11DeviceContext*	context)
{
	DirectX::CreateWICTextureFromFile(device, context, filePath, 0, &srv);

	D3D11_SAMPLER_DESC sampler_desc = {};
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // Tri-linear filtering
	sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampler_desc.MaxAnisotropy = 16;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&sampler_desc, &sampler);
}

//-------------------------------------------------------------------
// Set texture values
//-------------------------------------------------------------------
void Material::LoadTexture(Texture* _texture) 
{
	texture = _texture;
	srv = _texture->GetSRV();
	sampler = _texture->GetSampler();
}
