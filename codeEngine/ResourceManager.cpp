//---------------------------------------------------------
// By Jishnu Girish
//---------------------------------------------------------
#include "ResourceManager.h"

#include <locale>
#include <codecvt>
#include "OBJ_Loader.h"
//--------------------------------------------------------------
// Global utility functions needed by resource manager
//--------------------------------------------------------------
//---------------------------------------------------------------
// Convert OBJ loader vertex to our custom vertex
//---------------------------------------------------------------
Vertex ObjlToVertex(objl::Vertex vertex)
{
	auto pos = XMFLOAT3(vertex.Position.X, vertex.Position.Y, vertex.Position.Z);
	auto normal = XMFLOAT3(vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z);
	auto uv = XMFLOAT2(vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y);
	return { pos, normal, uv };
}

//------------------------------------------------------------------------------
// Convert an array/vector of OBJ loader vertices to our custom vertex array
//------------------------------------------------------------------------------
std::vector<Vertex> ObjlToVertex(std::vector<objl::Vertex> vertices)
{
	std::vector<Vertex> verts;
	for (auto v : vertices)
	{
		verts.push_back(ObjlToVertex(v));
	}
	return verts;
}

//------------------------------------------------------------------------------
// Initialize resource manager
//------------------------------------------------------------------------------
ResourceManager::ResourceManager(ID3D11Device * _device, ID3D11DeviceContext* _context, AudioEngine* _audioEngine) 
	: device(_device), context(_context), audioEngine(_audioEngine)
{
}


ResourceManager::~ResourceManager()
{
	UnloadAll();
}

//---------------------------------------------------------
// Load All Resources from config file
//---------------------------------------------------------
void ResourceManager::LoadFromConfig()
{
	Configuration::GetInstance().LoadConfig();

	// Load Meshs
	auto meshes = Configuration::GetInstance().GetMeshFileList();
	for (auto const &mesh : meshes)
	{
		this->LoadMesh(mesh);
	}

	// Load Textures
	auto textures = Configuration::GetInstance().GetTextureFileList();
	for (auto const &texture : textures)
	{
		this->LoadTexture(StrToWstr(texture));
	}

	// Load Vertex shaders
	auto vshaders = Configuration::GetInstance().GetVShaderFileList();
	for (auto vShader : vshaders)
	{
		this->LoadVertexShader(StrToWstr(vShader));
	}

	// Load Vertex shaders
	auto pshaders = Configuration::GetInstance().GetPShaderFileList();
	for (auto pShader : pshaders)
	{
		this->LoadPixelShader(StrToWstr(pShader));
	}

	// Load Sounds
	auto sounds = Configuration::GetInstance().GetSoundFileList();
	for (auto sound : sounds)
	{
		audioEngine->LoadSound(sound, true, true, true);
	}
}

//---------------------------------------------------------
// Load a mesh resource
//---------------------------------------------------------
void ResourceManager::LoadMesh(const std::string& filePath)
{
	auto foundIt = std::find(uniqueIdString.begin(), uniqueIdString.end(), filePath);	// Check if the file was previously loaded
	if (foundIt != uniqueIdString.end())												// Do not load a file if it was previously loaded
		return;

	auto pathEnd = filePath.find_last_of("/");						// find the path of the file
	auto name = filePath.substr(pathEnd + 1, filePath.size());		// extract the file name from the path with extention
	
	objl::Loader loader;
	loader.LoadFile(filePath);
	//for (auto mesh : loader.LoadedMeshes)
	//{
		auto verts = ObjlToVertex(loader.LoadedVertices);
		auto indices = loader.LoadedIndices;
		Mesh* newMesh = new Mesh(verts.data(), verts.size(), indices.data(), indices.size(), device);
		if (newMesh->GetVertexBuffer() == nullptr)						// Check if loaded correctly
		{
			std::cout << " Could not load mesh " << std::endl;
			return;
		}
		meshResources[name] = std::make_shared< Resource<Mesh>>(filePath, name, newMesh);	// Add reference to loaded file
		uniqueIdString.emplace_back(filePath);												// Add reference to global resource db
	//}

}

//---------------------------------------------------------
// Return a mesh resource which has already been loaded
//---------------------------------------------------------
Mesh* ResourceManager::GetMeshByName(const std::string& _name)
{
	auto foundIt = meshResources.find(_name);
	if (foundIt == meshResources.end())
	{
		std::cout << "could not find mesh with name " << _name << std::endl;
		std::cout << "Check if the mesh was loaded " << std::endl;
		return nullptr;
	}
	return meshResources[_name]->GetResource();
}

//---------------------------------------------------------
// Load a texture resource
// Can load only png,jpg,or dds textures
//---------------------------------------------------------
void ResourceManager::LoadTexture(const std::wstring& filePath) 
{
	auto foundIt = std::find(uniqueIdString.begin(), uniqueIdString.end(), WstrToStr(filePath));	// Check if the file was previously loaded
	if (foundIt != uniqueIdString.end())												// Do not load a file if it was previously loaded
		return;

	auto pathEnd = filePath.find_last_of(L"/");						// find the path of the file
	auto name = filePath.substr(pathEnd + 1, filePath.size());		// extract the file name from the path with extention

	Texture * newTexture = new Texture(filePath.c_str(), device, context);			// Load in the new mesh
	if (newTexture->GetSRV() == nullptr)											// Check if loaded correctly
	{
		std::cout << " Could not load texture " << filePath.c_str() << std::endl;
		return;
	}
	textureResource[name] = std::make_shared< Resource<Texture>>(WstrToStr(filePath), WstrToStr(name), newTexture);	// Add reference to loaded file
	uniqueIdString.emplace_back(WstrToStr(filePath));																// Add reference to global resource db
}

//---------------------------------------------------------
// Return a texture resource that has already been loaded
//---------------------------------------------------------
Texture * ResourceManager::GetTextureByName(const std::wstring& _name)
{
	auto foundIt = textureResource.find(_name);
	if (foundIt == textureResource.end())
	{
		std::cout << "could not find texture with name " << _name.c_str() << std::endl;
		std::cout << "Check if the texture was loaded " << std::endl;
		return nullptr;
	}
	return textureResource[_name]->GetResource();
}

//---------------------------------------------------------
// Loas a vextex shader as a .cso file
//---------------------------------------------------------
void ResourceManager::LoadVertexShader(const std::wstring& filePath)
{
	auto foundIt = std::find(uniqueIdString.begin(), uniqueIdString.end(), WstrToStr(filePath));	// Check if the file was previously loaded
	if (foundIt != uniqueIdString.end())															// Do not load a file if it was previously loaded
		return;

	auto pathEnd = filePath.find_last_of(L"/");								// find the path of the file
	auto name = filePath.substr(pathEnd + 1, filePath.size());				// extract the file name from the path with extention

	VertexShader * newVShader = new VertexShader(filePath.c_str(),device,context);			// Load in the new mesh
	if (newVShader->Get() == nullptr)										// Check if loaded correctly
	{
		std::cout << " Could not load vertex shader " << filePath.c_str() << std::endl;
		return;
	}
	VSResources[name] = std::make_shared< Resource<VertexShader>>(WstrToStr(filePath), WstrToStr(name), newVShader);	// Add reference to loaded file
	uniqueIdString.emplace_back(WstrToStr(filePath));																	// Add reference to global resource db
}

//---------------------------------------------------------
// Load a pixel shader as a .cso file
//---------------------------------------------------------
void ResourceManager::LoadPixelShader(const std::wstring& filePath)
{
	auto foundIt = std::find(uniqueIdString.begin(), uniqueIdString.end(), WstrToStr(filePath));	// Check if the file was previously loaded
	if (foundIt != uniqueIdString.end())															// Do not load a file if it was previously loaded
		return;

	auto pathEnd = filePath.find_last_of(L"/");								// find the path of the file
	auto name = filePath.substr(pathEnd + 1, filePath.size());				// extract the file name from the path with extention

	PixelShader * newPShader = new PixelShader(filePath.c_str(), device, context);			// Load in the new mesh
	if (newPShader->Get() == nullptr)										// Check if loaded correctly
	{
		std::cout << " Could not load pixel shader " << filePath.c_str() << std::endl;
		return;
	}
	PSResources[name] = std::make_shared< Resource<PixelShader>>(WstrToStr(filePath), WstrToStr(name), newPShader);	// Add reference to loaded file
	uniqueIdString.emplace_back(WstrToStr(filePath));																	// Add reference to global resource db
}

//---------------------------------------------------------
// Return a loaded vertex shader
//---------------------------------------------------------
SimpleVertexShader * ResourceManager::GetVShaderByName(const std::wstring & _name)
{
	auto foundIt = VSResources.find(_name);
	if (foundIt == VSResources.end())
	{
		std::cout << "could not find Vertex shader with name " << _name.c_str() << std::endl;
		std::cout << "Check if the texture was loaded " << std::endl;
		return nullptr;
	}
	return VSResources[_name]->GetResource()->Get();
}

//---------------------------------------------------------
// Retrun a loaded pixel shader
//---------------------------------------------------------
SimplePixelShader * ResourceManager::GetPShaderByName(const std::wstring & _name)
{
	auto foundIt = PSResources.find(_name);
	if (foundIt == PSResources.end())
	{
		std::cout << "could not find PIxel shader with name " << _name.c_str() << std::endl;
		std::cout << "Check if the texture was loaded " << std::endl;
		return nullptr;
	}
	return PSResources[_name]->GetResource()->Get();
}


//---------------------------------------------------------
// Unload a given resource
// Can unload only png,jpg,or dds textures
//---------------------------------------------------------
bool ResourceManager::UnloadResource(std::string _name)
{
	auto period = _name.find_last_of(".");
	auto ext = _name.substr(period + 1, _name.size());
	if(ext == "obj")
	{	
		auto foundIt = meshResources.find(_name);
		if (foundIt == meshResources.end())
		{
			std::cout << "The resource isn't loaded to unload" << std::endl;
			return false;
		}
		meshResources.erase(foundIt);
		return true;
	}
	else if(ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "dds")
	{
		auto foundIt = textureResource.find(StrToWstr(_name));
		if (foundIt == textureResource.end())
		{
			std::cout << "The resource isn't loaded to unload" << std::endl;
			return false;
		}
		textureResource.erase(foundIt);
		return true;
	}
	else if(ext == "cso")
	{
		auto foundIt0 = VSResources.find(StrToWstr(_name));
		if (foundIt0 == VSResources.end())
		{
			auto foundIt1 = PSResources.find(StrToWstr(_name));
			if (foundIt1 == PSResources.end())
			{
				std::cout << "The resource isn't loaded to unload" << std::endl;
				return false;
			}
			PSResources.erase(foundIt1);
			return true;
		}
		VSResources.erase(foundIt0);
		return true;
		
	}
	else 
	{
		std::cout << "Enter the right extension for the resource" << std::endl;
	}
		return true;
}

//---------------------------------------------------------
// Unload all loaded resources
// Can unload only png,jpg,or dds textures
//---------------------------------------------------------
void ResourceManager::UnloadAll()
{
	//Unload vertex shaders
	for (auto x : VSResources)
	{
		delete x.second->GetResource()->Get();
	}

	//Unload pixel shaders
	for (auto x : PSResources)
	{
		delete x.second->GetResource()->Get();
	}

	//Unload vertex shader resource component
	for (auto x : VSResources)
	{
		delete x.second->GetResource();
	}

	//Unload pixel shader resource component
	for (auto x : PSResources)
	{
		delete x.second->GetResource();
	}

	//Unload Meshs
	for (auto x : meshResources)
	{
		delete x.second->GetResource();
	}

	//Unload Textures
	for (auto x : textureResource)
	{
		delete x.second->GetResource();
	}

	meshResources.clear();
	textureResource.clear();
	VSResources.clear();
	PSResources.clear();
}

//---------------------------------------------------------------
// String conversions
//---------------------------------------------------------------


//---------------------------------------------------------------
// Convert string to wide string
//---------------------------------------------------------------
std::wstring ResourceManager::StrToWstr(const std::string& s)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(s);
}

//---------------------------------------------------------------
// Convert wide string to string
//---------------------------------------------------------------
std::string ResourceManager::WstrToStr(const std::wstring& s)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(s);
}


