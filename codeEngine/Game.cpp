#include "Game.h"
// For the DirectX Math library
using namespace DirectX;

// Create toggle buttons here
KeyToggle ToggleEnter(VK_RETURN);
KeyToggle ToggleTab(VK_TAB);
KeyToggle ToggleShift(VK_SHIFT);
KeyToggle ToggleBKspc(VK_BACK);

BOOL CALLBACK EnumWindowTitles(HWND hwnd, LPARAM lParam) {
	const DWORD TITLE_SIZE = 1024;
	WCHAR windowTitle[TITLE_SIZE];

	GetWindowTextW(hwnd, windowTitle, TITLE_SIZE);

	int length = ::GetWindowTextLength(hwnd);
	wstring title(&windowTitle[0]);
	if (!IsWindowVisible(hwnd) || length == 0 || title == L"Program Manager") {
		return TRUE;
	}

	// Retrieve the pointer passed into this callback, and re-'type' it.
	// The only way for a C API to pass arbitrary data is by means of a void*.
	std::vector<std::wstring>& titles =
		*reinterpret_cast<std::vector<std::wstring>*>(lParam);
	titles.push_back(title);

	return TRUE;
}

// -------------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// -------------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.\n");
#endif
	
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Release any (and all!) DirectX objects
	// we've made in the Game class
	
	//Delete Camera
	if (camera != nullptr)
		delete camera;
	//Delete Renderer
	if (renderer != nullptr)
		delete renderer;
	//Delete game objects
	for (auto gameobjects : gameObjects)
	{
		delete gameobjects;
	}

	//Resource manager is deleting the resources
	//Delete meshes
	//for (auto mesh : meshes)
	//{
		//delete mesh;
	//}

	//Delete materials
	for (auto material : materials)
	{
		delete material;
	}
	
	delete canvas;
	delete dialogueSystem;
	delete rm;
	// Delete audio resources
	audioEngine->ShutDown();

	// Skybox
	skyRastState->Release();
	skyDepthState->Release();

	// Delete profiler
	auto endGlobalTime = GetTickCount();
	Profiler::DeleteInstance(endGlobalTime - startGlobalTime);

	// Unregister tools
	ToolsManager::Instance().UnregisterAll();

	//Delete ship
	delete Ship;

	//Lifebar deletion is handled in UICanvas
}

void Game::PlayObjectSound() 
{
	//std::cout << "Red button" << std::endl;
	gameObjects[1]->PlayObjectSound(0, 3);
}
void Game::StopObjectSound()
{
	//std::cout << "Red button" << std::endl;
	gameObjects[1]->StopObjectSound(0,3);
}

void Game::Print(vector<wstring> parameters) {
	for (wstring wstr : parameters) {
		string str = string(wstr.begin(), wstr.end());
		cout << str << endl;
	}
}

void Game::SetWireframeMode(bool set) {
	renderer->SetWireframeMode(set);
}

bool CheckLastParameter(vector<wstring> &parameters, vector<wstring>::iterator &it, wstring name) {
	if (it == parameters.begin()) {
		return false;
	}
	if (*(it - 1) != name) {
		return false;
	}
	return true;
}

//stof can parse	0
//stof can parse	0.1
//stof can parse	.1
//stof cannot parse	.
//If the string is only . return false
bool CheckIsNumber(wstring str) {
	for (int i = 0; i < str.length(); i++) {
		if (!(isdigit(str[i]) || (str[i] == L'.' && str.length() != 1))) {
			return false;
		}
	}
	return true;
}

//May be more efficient to use a map
void Game::CreateObject(vector<wstring> parameters) {
	//Default shape is cube if no /shape parameter is specified
	string shape = "cube";
	//World position 0, 0, 0, if no /x /y and /z are specified
	float x = 0, y = 0, z = 0;
	for (auto it = parameters.begin(); it != parameters.end(); it++) {
		//Check for /shape parameter
		bool paramExists = CheckLastParameter(parameters, it, L"/shape");
		if (paramExists) {
			if (*it == L"box" || *it == L"cube") {
				shape = "cube";
			}
			else if (*it == L"sphere") {
				shape = "sphere";
			}
			else if (*it == L"cone") {
				shape = "cone";
			}
			else if (*it == L"helix") {
				shape = "helix";
			}
			else if (*it == L"torus") {
				shape = "torus";
			}
		}
		else {
		}

		//Check for /x parameter
		paramExists = CheckLastParameter(parameters, it, L"/x");
		if (paramExists) {
			if (CheckIsNumber(*it)) {
				x = stof(*it);
			}
		}

		//Check for /y parameter
		paramExists = CheckLastParameter(parameters, it, L"/y");
		if (paramExists) {
			if (CheckIsNumber(*it)) {
				y = stof(*it);
			}
		}

		//Check for /z parameter
		paramExists = CheckLastParameter(parameters, it, L"/z");
		if (paramExists) {
			if (CheckIsNumber(*it)) {
				z = stof(*it);
			}
		}
	}
	GameObject *obj = new GameObject(rm->GetMeshByName(shape + ".obj"), materials[0]);
	obj->SetObjectPosition(XMFLOAT3(x, y, z));
	obj->UpdateWorldMatrix();
	gameObjects.push_back(obj);
}

void Game::ThirdPCam()
{
	camera->SetType(ThirdPersonCam);
}

void Game::FreeCam()
{
	camera->SetType(FreeFormCam);
}

void Game::BBoxOn()
{
	boundingBoxEnable = true;
}

void Game::BBoxOff()
{
	boundingBoxEnable = false;
}

void Game::ChangeSky()
{
	currentSky = (currentSky + 1) % skyTextures.size();
}

void Game::CreateDebugFunctions() {
	canvas->CreateDebugFunction(L"WireFrameOn", [&](vector<wstring> parameters) {SetWireframeMode(true); });
	canvas->CreateDebugFunction(L"WireFrameOff", [&](vector<wstring> parameters) {SetWireframeMode(false); });
	canvas->CreateDebugFunction(L"BoundingBoxOn", [&](vector<wstring> parameters) { BBoxOn(); });
	canvas->CreateDebugFunction(L"BoundingBoxOff", [&](vector<wstring> parameters) { BBoxOff(); });
	canvas->CreateDebugFunction(L"CreateObject", [&](vector<wstring> parameters) {CreateObject(parameters); });
	canvas->CreateDebugFunction(L"Print", [&](vector<wstring> parameters) {Print(parameters); });
	canvas->CreateDebugFunction(L"CanvasOn", [&](vector<wstring> parameters) { canvas->Enable(true); });
	canvas->CreateDebugFunction(L"CanvasOff", [&](vector<wstring> parameters) { canvas->Enable(false); });
	canvas->CreateDebugFunction(L"Quit", [&](vector<wstring> parameters) {Quit(); });
	canvas->CreateDebugFunction(L"Exit", [&](vector<wstring> parameters) {Quit(); });
	canvas->CreateDebugFunction(L"ThirdPersonCam", [&](vector<wstring> parameters) { ThirdPCam(); });
	canvas->CreateDebugFunction(L"FreeCam", [&](vector<wstring> parameters) { FreeCam(); });
	canvas->CreateDebugFunction(L"ChangeSky", [&](vector<wstring> parameters) { ChangeSky(); });
}

void Game::StartTool() 
{
	std::vector<std::wstring> titles;
	EnumWindows(EnumWindowTitles, reinterpret_cast<LPARAM>(&titles));			// Find all open window titles
	
	if (std::find(titles.begin(), titles.end(), L"SoundEditor") == titles.end())
	{
		ToolsManager::Instance().StartTool("SoundEditor.exe");
	}
}
int env = 0;
void Game::ChangeSoundEnvironment()
{
	audioEngine->ActivateReverb(false);
	env++;
	if (env >= rProps.size())
	{
		env = 0;
	}
	auto prop = audioEngine->SetReverbProperties(rProps[env]);
	audioEngine->SetEnvironmentReverb(prop, AudioVector3{ 0,0,0 }, 0.0f, 1000.0f);
	audioEngine->ActivateReverb(true);
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Initialize audio
	audioEngine->Init();
	//Initialize the game
	LoadResources();
	CreateCamera();
	camera->SetType(ThirdPersonCam);
	CreateRenderer();
	CreateLights();

	// Initialize reverb
	SetReverbPresets();

	auto prop = audioEngine->SetReverbProperties(rProps[0]);
	audioEngine->SetEnvironmentReverb(prop, AudioVector3{ 0,0,0 }, 0.0f, 1000.0f);
	audioEngine->ActivateReverb(false);
	audioEngine->Set3dListenerAndOrientation(AudioVector3{ camera->GetPosition().x,camera->GetPosition().y,camera->GetPosition().z },	// Listener position in on the camera
											 AudioVector3{ 0,0,1 }, AudioVector3{ 0,1,0 });												// Listener on th camera
	
	
	// Create objects in the game
	CreateGameObjects();

	// Create UI canvas
	CreateCanvas();

	//RegisterTools
#ifdef CE_DEBUG
	ToolsManager::Instance().RegisterTool("..\\..\\SoundEditor\\bin\\Debug\\SoundEditor.exe");
#else
	ToolsManager::Instance().RegisterTool("SoundEditor\\SoundEditor.exe");
#endif // CE_DEBUG

	
	// Create functions for the UI window
	CreateDebugFunctions();

	// Create states for sky rendering
	D3D11_RASTERIZER_DESC rs = {};
	rs.CullMode = D3D11_CULL_FRONT;
	rs.FillMode = D3D11_FILL_SOLID;
	device->CreateRasterizerState(&rs, &skyRastState);

	D3D11_DEPTH_STENCIL_DESC ds = {};
	ds.DepthEnable = true;
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&ds, &skyDepthState);

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Game::SetReverbPresets()
{
	rProps.push_back(ENVIRONMENT_UNDERWATER);
	rProps.push_back(ENVIRONMENT_GENERIC);
	rProps.push_back(ENVIRONMENT_PADDEDCELL);
	rProps.push_back(ENVIRONMENT_ROOM);
	rProps.push_back(ENVIRONMENT_BATHROOM);
	rProps.push_back(ENVIRONMENT_LIVINGROOM);
	rProps.push_back(ENVIRONMENT_STONEROOM);
	rProps.push_back(ENVIRONMENT_AUDITORIUM);
	rProps.push_back(ENVIRONMENT_CONCERTHALL);
	rProps.push_back(ENVIRONMENT_CAVE);
	rProps.push_back(ENVIRONMENT_ARENA);
	rProps.push_back(ENVIRONMENT_HANGER);
	rProps.push_back(ENVIRONMENT_CARPETTEDHALLWAY);
	rProps.push_back(ENVIRONMENT_HALLWAY);
	rProps.push_back(ENVIRONMENT_STONECORRIDOR);
	rProps.push_back(ENVIRONMENT_ALLEY);
	rProps.push_back(ENVIRONMENT_FOREST);
	rProps.push_back(ENVIRONMENT_CITY);
	rProps.push_back(ENVIRONMENT_MOUNTAINS);
	rProps.push_back(ENVIRONMENT_QUARRY);
	rProps.push_back(ENVIRONMENT_PLAIN);
	rProps.push_back(ENVIRONMENT_PARKINGLOT);
	rProps.push_back(ENVIRONMENT_SEWERPIPE);
	rProps.push_back(ENVIRONMENT_REVERB_OFF);

	envNames.push_back(L"ENVIRONMENT_UNDERWATER");
	envNames.push_back(L"ENVIRONMENT_GENERIC");
	envNames.push_back(L"ENVIRONMENT_PADDEDCELL");
	envNames.push_back(L"ENVIRONMENT_ROOM");
	envNames.push_back(L"ENVIRONMENT_BATHROOM");
	envNames.push_back(L"ENVIRONMENT_LIVINGROOM");
	envNames.push_back(L"ENVIRONMENT_STONEROOM");
	envNames.push_back(L"ENVIRONMENT_AUDITORIUM");
	envNames.push_back(L"ENVIRONMENT_CONCERTHALL");
	envNames.push_back(L"ENVIRONMENT_CAVE");
	envNames.push_back(L"ENVIRONMENT_ARENA");
	envNames.push_back(L"ENVIRONMENT_HANGER");
	envNames.push_back(L"ENVIRONMENT_CARPETTEDHALLWAY");
	envNames.push_back(L"ENVIRONMENT_HALLWAY");
	envNames.push_back(L"ENVIRONMENT_STONECORRIDOR");
	envNames.push_back(L"ENVIRONMENT_ALLEY");
	envNames.push_back(L"ENVIRONMENT_FOREST");
	envNames.push_back(L"ENVIRONMENT_CITY");
	envNames.push_back(L"ENVIRONMENT_MOUNTAINS");
	envNames.push_back(L"ENVIRONMENT_QUARRY");
	envNames.push_back(L"ENVIRONMENT_PLAIN");
	envNames.push_back(L"ENVIRONMENT_PARKINGLOT");
	envNames.push_back(L"ENVIRONMENT_SEWERPIPE");
	envNames.push_back(L"ENVIRONMENT_REVERB_OFF");
}

// --------------------------------------------------------
// Load Resources
// --------------------------------------------------------
void Game::LoadResources()
{
	// Initialize resource manager
	rm = new ResourceManager(device, context,audioEngine);
	rm->LoadFromConfig();
}

// ---------------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// ---------------------------------------------------------------
void Game::CreateCamera()
{
	camera = new Camera();
	camera->UpdateViewMatrix();
	camera->UpdateProjectionMatrix(width, height);

}

//---------------------------------------------------------------
// Initialize the renderer
//---------------------------------------------------------------
void Game::CreateRenderer()
{
	renderer = new Renderer(depthStencilView, backBufferRTV, context, swapChain, camera, device);
}

// ---------------------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// ---------------------------------------------------------------------
void Game::CreateGameObjects()
{
	// Load audio
#ifdef CE_DEBUG
	audioEngine->LoadSound("../../Assets/Sounds/explosion.wav", true, false);
#else 
	audioEngine->LoadSound("Assets/Sounds/explosion.wav", true, false);
#endif

	// Skybox
	skyTextures.push_back(rm->GetTextureByName(L"Space.dds"));
	skyTextures.push_back(rm->GetTextureByName(L"SunnyCubeMap.dds"));

	// Create materials
	materials.push_back(new Material(rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"), rm->GetTextureByName(L"texture01.jpg")));
	materials.push_back(new Material(rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"), rm->GetTextureByName(L"texture01.png")));
	materials.push_back(new Material(rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"), rm->GetTextureByName(L"steel.jpg")));
	materials.push_back(new Material(rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"), rm->GetTextureByName(L"Aircraft_diffuse.jpg"), rm->GetTextureByName(L"Aircraft_Normals.jpg")));
	
	meshes.push_back(rm->GetMeshByName("cube.obj"));
	meshes.push_back(rm->GetMeshByName("sphere.obj"));
	meshes.push_back(rm->GetMeshByName("cone.obj"));
	meshes.push_back(rm->GetMeshByName("helix.obj"));
	meshes.push_back(rm->GetMeshByName("torus.obj"));
	meshes.push_back(rm->GetMeshByName("Aircraft.obj"));

	// Ship with bounding box
	Ship = new ThirdPersonObject(meshes[5], materials[3],camera,true);
	Ship->SetBboxMesh(device);
	Ship->SetObjectPosition(XMFLOAT3(0.f,0.f,-5.f));
	Ship->Rotate(0.0, 179.0, 0.0);	
	Ship->UpdateWorldMatrix();

	// Sound to be attached to an object
	std::vector<std::string> sound;
#ifdef CE_DEBUG
	sound.push_back("../../Assets/Sounds/drumloop.wav");
#else
	sound.push_back("Assets/Sounds/drumloop.wav");
#endif
	// Initialize game objects with bounding box / spheres
	gameObjects.push_back(new GameObject(meshes[0], materials[0],true));
	gameObjects.push_back(new GameObject(meshes[1], materials[1], sound, audioEngine,false,true));
	gameObjects.push_back(new GameObject(meshes[2], materials[1],true));
	gameObjects.push_back(new GameObject(meshes[3], materials[0],true));
	gameObjects.push_back(new GameObject(meshes[4], materials[0],true));

	// Set bounding boxes before altering the objects
	for (auto g : gameObjects)
	{
		g->SetBboxMesh(device);
	}
	// Position game objects
	gameObjects[1]->SetObjectPosition(XMFLOAT3(-1.5, 0, 0));
	gameObjects[1]->UpdateWorldMatrix();

	gameObjects[2]->SetObjectPosition(XMFLOAT3(1.5, 0, 0));
	gameObjects[2]->UpdateWorldMatrix();

	gameObjects[3]->SetObjectPosition(XMFLOAT3(3, 0, 0));
	gameObjects[3]->UpdateWorldMatrix();

	gameObjects[4]->SetObjectPosition(XMFLOAT3(-3, 0, 0));
	gameObjects[4]->UpdateWorldMatrix();

}

// -------------------------------------------------------------------
// Create lights in the scene and assigns it to the appropriate shader
// -------------------------------------------------------------------
void Game::CreateLights()
{
	// Initializing lights in the scene
	dLight1.ambientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	dLight1.diffuseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	dLight1.direction = XMFLOAT3(-1.0f, 1.0f, 0.0f);

	dLight2.ambientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	dLight2.diffuseColor = XMFLOAT4(0.f, 0.2f, 0.3f, 1.0f);
	dLight2.direction = XMFLOAT3(1.f, -1.f, 0.f);

	pLight.color = XMFLOAT4(0.3f, 0.3f, 0.f, 1.f);
	pLight.position = XMFLOAT3(0.f, 1.0f, 0.f);

	// Setting the lights to the shader
	rm->GetPShaderByName(L"PixelShader1.cso")->SetData(
		"dLight1",  // The name of the (eventual) variable in the shader
		&dLight1,   // The address of the data to copy
		sizeof(DirectionalLight)); // The size of the data to copy

	rm->GetPShaderByName(L"PixelShader1.cso")->SetData(
		"dLight2",  // The name of the (eventual) variable in the shader
		&dLight2,   // The address of the data to copy
		sizeof(DirectionalLight)); // The size of the data to copy

	rm->GetPShaderByName(L"PixelShader1.cso")->SetData(
		"pLight",  // The name of the (eventual) variable in the shader
		&pLight,   // The address of the data to copy
		sizeof(PointLight)); // The size of the data to copy
	
}

int id = 0;
int fileNumber = 0;

// -------------------------------------------------------------
// Function to handle sound playback in the game
// -------------------------------------------------------------
void Game::PlaySounds()
{
	PROFILE

	SoundProperties newSound;

	if (KeyInput::IsKeyDown(KEY_O))
	{
		audioEngine->StopChannel(id,5);		// fade out to zero volume in 5 seconds
		id--;
	}
	if (ToggleTab)
	{
		Configuration::GetInstance().LoadSoundConfigList();

		audioEngine->StopAllChannels();		// Stop all channels without any fade
		id = audioEngine->PlaySounds(Configuration::GetInstance().GetSoundProp()[fileNumber].name, AudioVector3{ 0, 0, 0 }, audioEngine->VolumeToDecibal(Configuration::GetInstance().GetSoundProp()[fileNumber].volume), 3);
		audioEngine->SetPitch(id, Configuration::GetInstance().GetSoundProp()[fileNumber].pitch);
		if ((fileNumber + 1) == Configuration::GetInstance().GetSoundProp().size())	// loop if last sound is played
			fileNumber = 0;
		else
			fileNumber++;
	}
}


//----------------------------------------------------------
// Create UI canvas and UI elements on the canvas
//----------------------------------------------------------
void Game::CreateCanvas() {
	PROFILE
	canvas = new UICanvas(device, context, rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"));
	dialogueSystem = new DialogueSystem(device, context, rm, width, height);
	//No longer a need for these since you can cast a Color to an XMFLOAT4
	//XMFLOAT4 white = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	//XMFLOAT4 green = XMFLOAT4(0.0f, 0.5f, 0.0f, 1.0f);
	//XMFLOAT4 gray = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//XMFLOAT4 blue = XMFLOAT4(0, 0, 0.5f, 1);

	//Load lifebar materials
	lifebarMaterials[0] = Material(rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"), rm->GetTextureByName(L"lifebar0.png"));
	lifebarMaterials[1] = Material(rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"), rm->GetTextureByName(L"lifebar1.png"));
	lifebarMaterials[2] = Material(rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"), rm->GetTextureByName(L"lifebar2.png"));
	lifebarMaterials[3] = Material(rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"), rm->GetTextureByName(L"lifebar3.png"));
	lifebarMaterials[4] = Material(rm->GetVShaderByName(L"VertexShader1.cso"), rm->GetPShaderByName(L"PixelShader1.cso"), rm->GetTextureByName(L"lifebar4.png"));

	lifebar = canvas->CreateObject(3.0f, 1.2f, 0.4f, 0.1f, &lifebarMaterials[4], (XMFLOAT4)Colors::White);

	UIButton *playButton = canvas->CreateButton(2.5f, -1.625f, 0.5f, 0.5f, materials[2], L"Play", (XMFLOAT4) Colors::Green);
	UIButton *stopButton = canvas->CreateButton(3.0f, -1.625f, 0.5f, 0.5f, materials[2], L"Stop", (XMFLOAT4) Colors::Red);
	UIButton *rotateLeftButton = canvas->CreateButton(-0.5f, 1.625, 1.0f, 0.5f, materials[2], L"Rotate helix\nCCW", (XMFLOAT4)Colors::LightBlue);
	UIButton *rotateRightButton = canvas->CreateButton(0.5f, 1.625, 1.0f, 0.5f, materials[2], L"Rotate helix\nCW", (XMFLOAT4)Colors::LightBlue);
	UIButton *launchSoundEditor = canvas->CreateButton(3.0f, -1.025f, 0.5f, 0.5f, materials[2], L"Tool", (XMFLOAT4)Colors::White);
	UIButton *changeReverb = canvas->CreateButton(2.5f, -1.025f, 0.5f, 0.5f, materials[2], L"Reverb", (XMFLOAT4)Colors::White);
	environment = canvas->CreateText(envNames[env],Left,50.0f,50.0f);
	
	playButton->AssignFunc([&] { PlayObjectSound(); });	// Calls PlayObjectSound when play button is clicked
	stopButton->AssignFunc([&] { StopObjectSound(); });	// Calls StopObjectSound when stop button is clicked
	rotateLeftButton->AssignFunc([&] {gameObjects[3]->Rotate(0, 0, 0.2f); });
	rotateRightButton->AssignFunc([&] {gameObjects[3]->Rotate(0, 0, -0.2f); });
	launchSoundEditor->AssignFunc([&] { StartTool(); });
	changeReverb->AssignFunc([&] { ChangeSoundEnvironment(); });
#ifdef CE_DEBUG
	objLoader.LoadObjectsFromFile("../../Assets/UIObjects.txt", rm, (*canvas), materials[0]);
	dialogueSystem->LoadLinesFromFile("../../Assets/Dialogue.txt");
#else
	objLoader.LoadObjectsFromFile("Assets/UIObjects.txt", rm, (*canvas), materials[0]);
	dialogueSystem->LoadLinesFromFile("Assets/Dialogue.txt");
#endif
}

// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// -------------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();
	renderer->SetDepthStencilView(depthStencilView);
	renderer->SetBackBuffer(backBufferRTV);
	camera->UpdateProjectionMatrix(width, height);
	dialogueSystem->UpdateWidthAndHeight(width, height);
}

// --------------------------------------------------------
// Move ship
// --------------------------------------------------------
void Game::MoveShip(float deltaTime)
{
	Ship->Update(deltaTime);
}
// --------------------------------------------------------
// Update game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	PROFILE
	// Update reverb environment text
	environment->Text = envNames[env];
 
	if (!canvas->DebugWindowActive())
	{
		PlaySounds();
		camera->UpdateFromInput(deltaTime);
	}
	dialogueSystem->Update(deltaTime);

	if (!stopAnimation)
	{
		float sinTime = sin(totalTime * 2);

		gameObjects[0]->Rotate(0,deltaTime,0);
	}

	if (!canvas->DebugWindowActive()) {
		// Move game object
		if (KeyInput::IsKeyDown(VK_RIGHT))
		{
			gameObjects[1]->Move( 10 * deltaTime, 0, 0);
			if (gameObjects[1]->GetBoundingSphere().Intersects(gameObjects[3]->GetBoundingBox()))
			{
				gameObjects[1]->Move( -10 * deltaTime,0, 0);
			}
		}
		if (KeyInput::IsKeyDown(VK_LEFT))
		{
			gameObjects[1]->Move( -10 * deltaTime,0, 0);
			if (gameObjects[1]->GetBoundingSphere().Intersects(gameObjects[3]->GetBoundingBox()))
			{
				gameObjects[1]->Move( 10 * deltaTime,0, 0);
			}
		}
		if (KeyInput::IsKeyDown(VK_UP))
		{
			gameObjects[1]->Move(0, 10 * deltaTime, 0);
			if (gameObjects[1]->GetBoundingSphere().Intersects(gameObjects[3]->GetBoundingBox()))
			{
				gameObjects[1]->Move(0,-10 * deltaTime, 0);
			}
		}
		if (KeyInput::IsKeyDown(VK_DOWN))
		{
			gameObjects[1]->Move(0,-10 * deltaTime, 0);
			if (gameObjects[1]->GetBoundingSphere().Intersects(gameObjects[3]->GetBoundingBox()))
			{
				gameObjects[1]->Move(0,10 * deltaTime, 0);
			}
		}

		if(camera->GetType() == ThirdPersonCam)
			MoveShip(deltaTime);
	}

	// Quit if the escape key is pressed
	if (KeyInput::IsKeyDown(VK_ESCAPE))
	{
		Quit();
	}
	if (KeyInput::WasKeyReleased('I'))
		stopAnimation = !stopAnimation;

	//Canvas and Debug Window interactions
	canvas->Update(deltaTime, totalTime);
	canvas->GetKeyInputString(KeyInput::GetKeyPressedWString());

	//Debug window state is checked in the functions
	if (KeyInput::WasKeyPressed(VK_RETURN)) {
		canvas->EnterPressed();
	}
	if (KeyInput::WasKeyPressed(VK_BACK)) {
		canvas->BackspacePressed();
	}
	if (KeyInput::WasKeyPressed(KEY_GRAVE)) {
		canvas->GravePressed();
	}
	if (KeyInput::WasKeyPressed(KEY_Z)) {
		if (!canvas->DebugWindowActive())
		{
			dialogueSystem->NextButton();
		}

	}
	if (KeyInput::WasKeyPressed(VK_SHIFT)) {
		if (!canvas->DebugWindowActive())
			dialogueSystem->SkipButton();
	}

	if (!Ship->GetIntersecting() && Ship->GetBoundingBox().Intersects(gameObjects[3]->GetBoundingBox())) {
		Ship->SetIntersecting(true);
		Ship->TakeDamage();
		lifebar->SetMaterial(&lifebarMaterials[Ship->GetLife()]);
#ifdef CE_DEBUG
		audioEngine->PlaySounds("../../Assets/Sounds/explosion.wav", AudioVector3{Ship->GetObjectPosition().x,Ship->GetObjectPosition().y,Ship->GetObjectPosition().z }, audioEngine->VolumeToDecibal(10), 0);
#else
		audioEngine->PlaySounds("Assets/Sounds/explosion.wav", AudioVector3{ Ship->GetObjectPosition().x,Ship->GetObjectPosition().y,Ship->GetObjectPosition().z }, audioEngine->VolumeToDecibal(10), 0);
#endif // CE_DEBUG

	}
	else if (Ship->GetIntersecting() && !Ship->GetBoundingBox().Intersects(gameObjects[3]->GetBoundingBox()) ){
		Ship->SetIntersecting(false);
	}

	//Audio engine interactions
	audioEngine->Set3dListenerAndOrientation(AudioVector3{ camera->GetPosition().x,camera->GetPosition().y,camera->GetPosition().z },		// Listener at camera position
											 AudioVector3{ camera->GetDirection().x,camera->GetDirection().y,camera->GetDirection().z },	// Listener forward direction = camera's forward direction
											 AudioVector3{ camera->GetUp().x,camera->GetUp().y,camera->GetUp().z });																		// Listener Up direction
	audioEngine->Update();

	//Without this, this was causing problems
	KeyInput::ResetKeys();
	
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	PROFILE
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
	renderer->Clear(color);

	// Draw each object
	for (auto gameObject : gameObjects)
	{
		renderer->Draw(gameObject);
		if (boundingBoxEnable)
		{
			renderer->DrawLines(gameObject, device);
			renderer->DrawSphere(gameObject,XMFLOAT4(0,1,0,0));
		}
	}
	// Draw Ship
	renderer->Draw(Ship);
	if (boundingBoxEnable)
		renderer->DrawLines(Ship,device);
	// Draw Sky
	renderer->RenderSky(meshes[0], rm->GetPShaderByName(L"SkyPS.cso"), rm->GetVShaderByName(L"SkyVS.cso"),
						skyTextures[currentSky]->GetSampler(), skyTextures[currentSky]->GetSRV(), skyRastState, skyDepthState);

	// Draw Canvas
	canvas->Draw(renderer);
	canvas->DrawSprites();

	dialogueSystem->Draw();

	// Present to screen
	renderer->PresentSwapChain();
}

#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
	if (!canvas->DebugWindowActive()) {
		canvas->OnMouseDown(x, y);
	}
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
	if (!canvas->DebugWindowActive())
		canvas->OnMouseUp();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (buttonState & 0x0001 && !canvas->DebugWindowActive())
	{  
		if (camera->GetType() == FreeFormCam)
		{
			camera->SetRotationY(((float)y - (float)prevMousePos.y) * camera->camSensitivity);
			camera->SetRotationX(((float)x - (float)prevMousePos.x) * camera->camSensitivity);
			camera->Rotate();
		}
		if (camera->GetType() == ThirdPersonCam)
		{
			Ship->Rotate(0, ((float)x - (float)prevMousePos.x) * (float)0.1, 0);
			camera->SetRotationX(((float)x - (float)prevMousePos.x) * camera->camSensitivity);
			camera->Rotate();
		}
		//std::cout << "\n" << camera->GetDirection().x << " " << camera->GetDirection().y << " " << camera->GetDirection().z ;
	}
	// Save the previous mouse position, so we have it for the future
	if (!canvas->DebugWindowActive()) {
		prevMousePos.x = x;
		prevMousePos.y = y;

		canvas->OnMouseMove(x, y, camera->GetProjectionMatrix(), camera->GetViewMatrix());
	}
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion