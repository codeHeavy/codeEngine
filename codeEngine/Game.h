#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include "DialogueSystem.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include "GameObject.h"
#include "Camera.h"
#include "Lights.h"
#include <vector>
#include "Renderer.h"
#include "UICanvas.h"
#include "UIObjectLoader.h"
#include "UserInputHandler.h"
#include "AudioEngine.h"
#include "Configuration.h"
#include "ResourceManager.h"
#include "ToolsManager.h"
#include "Vertex.h"
#include "WICTextureLoader.h"
#include "AudioEngine.h"
#include <functional>
#include <DDSTextureLoader.h>
#include "Profiler.h"
#include "ThirdPersonObject.h"
class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	// Inorder to stop aniamtion in update
	bool stopAnimation = false;
	// bounding box
	bool boundingBoxEnable = false;
	// current Skybox
	int currentSky = 0;

	//For loading objects into the canvas from an external file
	UIObjectLoader objLoader;
	
	// Components of the scene
	Renderer *renderer;
	Camera *camera;
	std::vector<Mesh*> meshes;
	std::vector<GameObject*> gameObjects;
	std::vector<Material*> materials;
	UICanvas *canvas;
	DialogueSystem *dialogueSystem;

	//Lifebar
	UIObject *lifebar;
	Material lifebarMaterials[5];

	// Lights in the scene
	DirectionalLight dLight1, dLight2;
	PointLight pLight;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	// Audio Engine
	AudioEngine *audioEngine;

	// Skybox
	ID3D11RasterizerState* skyRastState;
	ID3D11DepthStencilState* skyDepthState;
	
	void LoadResources();
	void CreateCamera();
	void CreateGameObjects();
	void CreateRenderer();
	void CreateLights();
	void CreateCanvas();
	void PlaySounds();
	void PlayObjectSound();
	void StopObjectSound();
	void CreateDebugFunctions();
	void SetReverbPresets();

	// Reverb demo
	vector<ReverbProperties> rProps;
	vector<std::wstring> envNames;

	// Resources
	ResourceManager *rm;
	std::vector<Texture*> skyTextures;

	//Debug functions
	void Print(vector<wstring> parameters);
	void SetWireframeMode(bool set);
	void CreateObject(vector<wstring> parameters);
	void BBoxOn();
	void BBoxOff();
	void ChangeSky();

	
	// Third person
	ThirdPersonObject* Ship;
	void MoveShip(float deltaTime);
	void ThirdPCam();
	void FreeCam();
	void StartTool();
	void ChangeSoundEnvironment();
	TextEntity *environment;
};

