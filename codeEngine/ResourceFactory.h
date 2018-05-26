#pragma once
#include "Mesh.h"
#include "Material.h"
#include "AudioEngine.h"
#include <map>
#include <vector>
#include "Texture.h"
#include "Shader.h"

template <typename T>
class Resource
{
private:
	std::string UID;
	std::string name;
	T *resource;
public:
	Resource(std::string _UID, std::string _name, T *_resource) : UID(_UID), name(_name), resource(_resource) {};
	~Resource() { };
	T* GetResource() 
	{ 
		if(resource != nullptr) 
			return resource; 
		return nullptr;
	};
};

