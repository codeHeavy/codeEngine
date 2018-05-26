#pragma once
#include "Mesh.h"
#include "Material.h"
#include "AudioEngine.h"
#include "ResourceFactory.h"
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include "Configuration.h"

typedef std::map<std::string , std::shared_ptr<Resource<Mesh>>			>	MeshesMap;
typedef std::map<std::wstring, std::shared_ptr<Resource<Texture>>		>	TexturesResourceMap;
typedef std::map<std::wstring, std::shared_ptr<Resource<VertexShader>>	>	VShaderResourceMap;
typedef std::map<std::wstring, std::shared_ptr<Resource<PixelShader>>	>	PShaderResourceMap;
typedef std::map<std::string , std::shared_ptr<Resource<Sound>>			>	SoundsResourceMap;

class ResourceManager
{
private:
	ID3D11Device * device;
	ID3D11DeviceContext* context;
	AudioEngine* audioEngine;
	std::vector<std::string> uniqueIdString;
	MeshesMap meshResources;
	TexturesResourceMap textureResource;
	VShaderResourceMap VSResources;
	PShaderResourceMap PSResources;
public:
	ResourceManager(ID3D11Device * _device, ID3D11DeviceContext* _context, AudioEngine* _audioEngine) ;
	~ResourceManager();
	void LoadMesh(const std::string& filePath);				// string file path
	void LoadTexture(const std::wstring& filePath);			// wide string needed fro textures
	void LoadVertexShader(const std::wstring& filePath);
	void LoadPixelShader(const std::wstring& filePath);
	void LoadFromConfig();

	void UnloadAll();
	bool UnloadResource(std::string _name);

	Mesh* GetMeshByName(const std::string& _name);
	Texture* GetTextureByName(const std::wstring& _name);			// all textures should be long strings
	SimpleVertexShader * GetVShaderByName(const std::wstring & _name);	// all shader files name s should be long strings
	SimplePixelShader * GetPShaderByName(const std::wstring & _name);		// all shader files name s should be long strings
	Sound GetSoundByName(const std::string& _name);

private:
	std::wstring StrToWstr(const std::string& s);
	std::string WstrToStr(const std::wstring& s);
};

