//UICanvas.h
//By Joshua Rand

#pragma once

#include "DebugWindow.h"
#include "CommonStates.h"
#include "GameObject.h"
#include <functional>
#include <map>
#include "Material.h"
#include "Mesh.h"
#include "Renderer.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include <string>
#include "UIButton.h"
#include "UIObject.h"
#include "UIRadioButton.h"
#include "Vertex.h"
#include <vector>
using namespace std;
using namespace DirectX;
using namespace SimpleMath;

const float FACTOR = 0.00146475f;

enum Alignment {
	Left = 0,
	Center,
	Right
};

struct TextEntity {
	wstring Text;
	Alignment Alignment;
	float x;
	float y;
};

class UICanvas {
public:
	UICanvas(ID3D11Device*, ID3D11DeviceContext*, SimpleVertexShader* vs, SimplePixelShader* ps);
	~UICanvas();
	//void Prepare(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix);
	void Draw(Renderer*);
	void DrawSprites();
	void Update(float deltaTime, float totalTime);
	void GetKeyInputString(wstring keyInput);
	void EnterPressed();
	void BackspacePressed();
	void GravePressed();
	void OnMouseMove(int x, int y, XMFLOAT4X4 projectionMatrix, XMFLOAT4X4 viewMatrix);
	void OnMouseDown(int x, int y);
	void OnMouseUp();
	UIObject * CreateObject(float x, float y, float width, float height, XMFLOAT4 color);
	UIObject * CreateObject(float x, float y, float width, float height, Material *material, XMFLOAT4 color);
	void CreateDebugFunction(wstring name, function<void(vector<wstring> parameters)> func);
	void Enable(bool enabled);
	UIButton * CreateButton(float x, float y, float width, float height, Material *material, wstring text, XMFLOAT4 color = XMFLOAT4(0,0,0,0));
	UIRadioButton * CreateRadioButton(float x, float y, float width, float height, XMFLOAT4 color, UIRadioGroup *radioGroup, Material *m);
	TextEntity * CreateText(wstring text, Alignment alignment, float x, float y);
	UIRadioGroup * CreateRadioGroup(int id);
	bool DebugWindowActive();
private:
	void CreateBox(ID3D11DeviceContext* context, SimpleVertexShader*, SimplePixelShader*);
	void DrawDebugWindow();
	XMFLOAT3 position;
	vector<UIObject*> UIObjects;
	map<int, UIRadioGroup*> radioGroups;
	vector<TextEntity*> textEntities;
	vector<Mesh*> boxMeshes;
	vector<Material*> materials;

	unique_ptr<SpriteBatch> spriteBatch;
	unique_ptr<SpriteFont> spriteFont;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	DebugWindow debugWindow;

	bool enabled;
};