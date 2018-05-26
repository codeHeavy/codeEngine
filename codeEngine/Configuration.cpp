//-----------------------------
// By Jishnu Girish
//-----------------------------

#include "Configuration.h"
#include <sstream>

Configuration::~Configuration()
{
}

//-------------------------------------------------------------------
// Load Consolidated config file
//-------------------------------------------------------------------
void Configuration::LoadConfig()
{
	std::string filename;
#ifdef CE_DEBUG
	std::ifstream configFile("../../Assets/config.txt");
#else
	std::ifstream configFile("Assets/config.txt");
#endif

	std::string line;
	try {
		if (configFile.is_open())
		{
			while (std::getline(configFile, line))
			{
				if (line == "")
					continue;
				std::istringstream sin(line.substr(line.find("=") + 1));
				if (line.find("Texture") != -1)						// Get texture data
				{
					sin >> filename;
					textureFileList.push_back(filename);
				}
				if (line.find("Mesh") != -1)						// Get mesh data
				{
					sin >> filename;
					meshFileList.push_back(filename);
				}
				if (line.find("VShader") != -1)						// Get vertex shader data
				{
					sin >> filename;
					vshaderFileList.push_back(filename);
				}
				if (line.find("PShader") != -1)						// Get pixel shader data
				{
					sin >> filename;
					pshaderFileList.push_back(filename);
				}
				if (line.find("Sound") != -1)						// Get sound file data
				{
					sin >> filename;
					soundFileList.push_back(filename);
				}
			}
			textureFileCount = textureFileList.size();
			meshFileCount = meshFileList.size();
			vshaderFileCount = vshaderFileList.size();
			pshaderFileCount = pshaderFileList.size();
			soundFileCount = soundFileList.size();
		}
		else
		{
			std::cout << "\n No config file found.\n";
			//return false;
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

//-------------------------------------------------------------------
// Read in texture files to be loaded
//-------------------------------------------------------------------
void Configuration::LoadTextureConfig()
{
	std::string filename;
#ifdef CE_DEBUG
	std::ifstream configFile("../../Assets/Texture/textureConfig.txt");
#else
	std::ifstream configFile("Assets/Texture/textureConfig.txt");
#endif
	std::string line;
	try {
		if (configFile.is_open())
		{
			while (std::getline(configFile, line))
			{
				if (line == "")
					continue;
				std::istringstream sin(line.substr(line.find("=") + 1));
				if (line.find("Texture") != -1)
				{
					sin >> filename;
					textureFileList.push_back(filename);
				}
			}
			textureFileCount = textureFileList.size();
		}
		else
		{
			std::cout << "\n No config file found.\n";
			//return false;
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

//-------------------------------------------------------------------
// Read in mesh files to be loaded
//-------------------------------------------------------------------
void Configuration::LoadMeshConfig()
{
	std::string filename;
#ifdef CE_DEBUG
	std::ifstream configFile("../../Assets/Models/meshConfig.txt");
#else
	std::ifstream configFile("Assets/Models/meshConfig.txt");
#endif
	std::string line;
	try
	{
		if (configFile.is_open())
		{
			while (std::getline(configFile, line))
			{
				if (line == "")
					continue;
				std::istringstream sin(line.substr(line.find("=") + 1));
				if (line.find("Mesh") != -1)
				{
					sin >> filename;
					meshFileList.push_back(filename);
				}
			}
			meshFileCount = meshFileList.size();
		}
		else
		{
			std::cout << "\n No config file found.\n";
			//return false;
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

//-------------------------------------------------------------------
// Load Sounds from a config file to a cache
//-------------------------------------------------------------------
void Configuration::LoadSoundConfig(AudioEngine *audioEngine)
{
	std::string filename;
#ifdef CE_DEBUG
	std::ifstream configFile("../../Assets/Sounds/soundConfig.txt");
#else
	std::ifstream configFile("Assets/Sounds/soundConfig.txt");
#endif
	std::string line;
	try {
		if (configFile.is_open())
		{
			while (std::getline(configFile, line))
			{
				if (line == "")
					continue;
				std::istringstream sin(line.substr(line.find("=") + 1));
				if (line.find("Sound") != -1)
				{
					sin >> filename;
					soundFileList.push_back(filename);
				}
			}
			soundFileCount = soundFileList.size();
			for (auto file : soundFileList)
			{
				audioEngine->LoadSound(file, true, true);
			}
		}
		else
		{
			std::cout << "\n No config file found.\n";
			//return false;
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

//-------------------------------------------------------------------
// Load sound properties
//-------------------------------------------------------------------
void Configuration::LoadSoundConfigList()
{
	soundConfigList.clear();
	ifstream soundConfig;
#ifdef CE_DEBUG
	soundConfig.open("../../Assets/Sounds/soundConfigList.txt");
#else
	soundConfig.open("Assets/Sounds/soundConfigList.txt");
#endif
	std::string line;
	SoundProperties newSound;
	try {
		if (soundConfig.is_open())
		{
			while (std::getline(soundConfig, line))
			{
				if (line == "" || line == " ")
					continue;
				auto comma_1 = line.find_first_of(',');
				auto name = line.substr(0, comma_1);							// get name
				newSound.name = name;											// assign name

				name = line.substr(comma_1 + 1, line.size());					// get unparsed str

				auto comma_2 = name.find_first_of(',');							// get next ,
				auto volume = name.substr(0, comma_2);							// get volume
				newSound.volume = (float)std::stof(volume);					// convert str to float

				auto pitch = line.substr(comma_1 + comma_2 + 2, line.size());	// get next unparsed str
				newSound.pitch = (float)std::stof(pitch);						// convert str to float
				soundConfigList.emplace_back(newSound);
			}
		}
		soundConfig.close();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

//-------------------------------------------------------------------
// Return sound properties from file
//-------------------------------------------------------------------
std::vector<SoundProperties> Configuration::GetSoundProp()
{
	return soundConfigList;
}

//-------------------------------------------------------------------
// Return the loaded sound file list
//-------------------------------------------------------------------
std::vector<std::string> Configuration::GetSoundFileList()
{
	return soundFileList;
}

//-------------------------------------------------------------------
// Return the number of sound files in the config file
//-------------------------------------------------------------------
unsigned __int64 Configuration::GetSoundFileCount()
{
	return soundFileCount;
}

//-------------------------------------------------------------------
// Return the loaded mesh file list
//-------------------------------------------------------------------
std::vector<std::string> Configuration::GetMeshFileList()
{
	return meshFileList;
}

//-------------------------------------------------------------------
// Return the number of mesh files in the config file
//-------------------------------------------------------------------
unsigned __int64 Configuration::GetMeshFileCount()
{
	return meshFileCount;
}

//-------------------------------------------------------------------
// Return the loaded texture file list
//-------------------------------------------------------------------
std::vector<std::string> Configuration::GetTextureFileList()
{
	return textureFileList;
}

//-------------------------------------------------------------------
// Return the number of texture files in the config file
//-------------------------------------------------------------------
unsigned __int64 Configuration::GetTextureFileCount()
{
	return textureFileCount;
}

//-------------------------------------------------------------------
// Return the loaded vertex shader file list
//-------------------------------------------------------------------
std::vector<std::string> Configuration::GetVShaderFileList()
{
	return vshaderFileList;
}

//-------------------------------------------------------------------
// Return the number of vertex shader files in the config file
//-------------------------------------------------------------------
unsigned __int64 Configuration::GetVShaderFileCount()
{
	return vshaderFileCount;
}

//-------------------------------------------------------------------
// Return the loaded pixel shader file list
//-------------------------------------------------------------------
std::vector<std::string> Configuration::GetPShaderFileList()
{
	return pshaderFileList;
}

//-------------------------------------------------------------------
// Return the number of pixel shader files in the config file
//-------------------------------------------------------------------
unsigned __int64 Configuration::GetPShaderFileCount()
{
	return pshaderFileCount;
}

//-------------------------------------------------------------------
// Checks if a file exists on a windows OS
//-------------------------------------------------------------------
bool Configuration::file_exists(const char* filename)
{
	DWORD fileAtt = GetFileAttributesA(filename);

	return (bool)((fileAtt == INVALID_FILE_ATTRIBUTES));
}