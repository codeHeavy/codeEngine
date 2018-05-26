#pragma once
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <string>
class Texture
{
private:
	ID3D11ShaderResourceView * srv;
	ID3D11SamplerState* sampler;
public:
	Texture(const std::wstring& filePath, ID3D11Device * _device, ID3D11DeviceContext * context);
	~Texture();
	ID3D11ShaderResourceView * GetSRV() const;
	ID3D11SamplerState* GetSampler() const;
private:
	void CreateCubeSampler(ID3D11Device * _device);
	void CreateTextureSampler(ID3D11Device * _device);
};

