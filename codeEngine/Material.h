#pragma once
#include "SimpleShader.h"
#include "WICTextureLoader.h"
#include "Texture.h"
#include "Shader.h"
class Material
{
private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	ID3D11ShaderResourceView* srv;
	ID3D11ShaderResourceView* normalsrv;
	ID3D11SamplerState* sampler;
	Texture *texture;
	Texture *normalTexture;
public:
	Material();
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader);
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11ShaderResourceView* srv,ID3D11SamplerState* sampler);
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, Texture* _texture);
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, Texture* _texture, Texture* _normalTex);
	~Material();
	SimplePixelShader* GetPixelShader();
	SimpleVertexShader* GetVertexShader();
	ID3D11ShaderResourceView* GetSRV();
	ID3D11ShaderResourceView* GetNormalSRV();
	ID3D11SamplerState* GetSampler();
	void LoadTexture(const wchar_t* filePath, ID3D11Device* device, ID3D11DeviceContext* context);
	void LoadTexture(Texture* texture);
};

