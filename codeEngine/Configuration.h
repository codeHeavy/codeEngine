//-----------------------------
// By Jishnu Girish
//-----------------------------
#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "AudioEngine.h"
#include <Windows.h>

struct SoundProperties {
	std::string name;
	float volume;
	float pitch;
};

class Configuration
{
private:
	std::vector<std::string> soundFileList;
	std::vector<std::string> meshFileList;
	std::vector<std::string> textureFileList;
	std::vector<std::string> vshaderFileList;
	std::vector<std::string> pshaderFileList;

	unsigned __int64 soundFileCount;
	unsigned __int64 meshFileCount;
	unsigned __int64 textureFileCount;
	unsigned __int64 vshaderFileCount;
	unsigned __int64 pshaderFileCount;

	std::vector<SoundProperties> soundConfigList;

	Configuration() {};
public:
	Configuration(Configuration const&) = delete;
	void operator=(Configuration const&) = delete;
	static Configuration& GetInstance()
	{
		static Configuration instance;
		return instance;
	}
	~Configuration();
	void LoadConfig();
	void LoadSoundConfig(AudioEngine *audioEngine);
	void LoadMeshConfig();
	void LoadTextureConfig();
	
	std::vector<std::string> GetSoundFileList();
	std::vector<std::string> GetMeshFileList();
	std::vector<std::string> GetTextureFileList();
	std::vector<std::string> GetVShaderFileList();
	std::vector<std::string> GetPShaderFileList();
	std::vector<SoundProperties> GetSoundProp();

	unsigned __int64 GetMeshFileCount();
	unsigned __int64 GetSoundFileCount();
	unsigned __int64 GetTextureFileCount();
	unsigned __int64 GetVShaderFileCount();
	unsigned __int64 GetPShaderFileCount();

	void LoadSoundConfigList();
	bool file_exists(const char* filename);
};

