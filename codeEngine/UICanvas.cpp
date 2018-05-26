//UICanvas.cpp
//By Joshua Rand

#include "UICanvas.h"

UICanvas::UICanvas(ID3D11Device* device, ID3D11DeviceContext* context, SimpleVertexShader* vs, SimplePixelShader* ps)
{
	position = XMFLOAT3(0.0f, 0.0f, -5.0f);
	this->device = device;
	this->context = context;
	this->vertexShader = vs;
	this->pixelShader = ps;
	this->enabled = true;

	spriteBatch.reset(new SpriteBatch(context));
#ifdef CE_DEBUG
	spriteFont.reset(new SpriteFont(device, L"../../Assets/Fonts/Calibri.spritefont"));
#else
	spriteFont.reset(new SpriteFont(device, L"Assets/Fonts/Calibri.spritefont"));
#endif
}

UICanvas::~UICanvas() {

	for (auto uiobj : UIObjects) {
		delete uiobj;
	}

	for (auto boxMesh : boxMeshes) {
		delete boxMesh;
	}

	for (auto material : materials) {
		delete material;
	}

	for (auto group : radioGroups) {
		delete group.second;
	}

	for (auto texts : textEntities) {
		delete texts;
	}

	//Necessary to delete Text fields in TextEntity?
	//for (auto textEntity : textEntities) {
	//	delete textEntity.Text;
	//}
}

UIObject * UICanvas::CreateObject(float x, float y, float width, float height, XMFLOAT4 color) {
	XMFLOAT3 fwd = XMFLOAT3(0.0f, 0.0f, -1.0f);
	XMFLOAT2 uv[] = { XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f),XMFLOAT2(0.0f, 1.0f) };
	Vertex vertices[] = {
		{ XMFLOAT3(x - width/2, y + height/2, 0.0f), fwd, uv[0] },
	{ XMFLOAT3(x + width/2, y + height/2, 0.0f), fwd, uv[1] },
	{ XMFLOAT3(x + width/2, y - height / 2, 0.0f), fwd, uv[2] },
	{ XMFLOAT3(x - width/2, y - height / 2, 0.0f), fwd, uv[3] },
	};

	UINT indices[] = { 0, 1, 2, 0, 2, 3 };
	Mesh * boxMesh = new Mesh(&vertices[0], 4, &indices[0], 6, device);
	Material * material = new Material(vertexShader, pixelShader);
	UIObject * box = new UIObject(boxMesh, material, x, y, width, height, color);//GameObject(boxMesh, material);
	box->SetObjectPosition(position);
	box->UpdateWorldMatrix();
	UIObjects.push_back(box);
	boxMeshes.push_back(boxMesh);
	materials.push_back(material);

	return box;
}

UIObject * UICanvas::CreateObject(float x, float y, float width, float height, Material * material, XMFLOAT4 color) 
{
	XMFLOAT3 fwd = XMFLOAT3(0.0f, 0.0f, -1.0f);
	XMFLOAT2 uv[] = { XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f),XMFLOAT2(0.0f, 1.0f) };
	Vertex vertices[] = {
		{ XMFLOAT3(x - width / 2, y + height / 2, 0.0f), fwd, uv[0] },
	{ XMFLOAT3(x + width / 2, y + height / 2, 0.0f), fwd, uv[1] },
	{ XMFLOAT3(x + width / 2, y - height / 2, 0.0f), fwd, uv[2] },
	{ XMFLOAT3(x - width / 2, y - height / 2, 0.0f), fwd, uv[3] },
	};

	UINT indices[] = { 0, 1, 2, 0, 2, 3 };
	Mesh * boxMesh = new Mesh(&vertices[0], 4, &indices[0], 6, device);
	UIObject * box = new UIObject(boxMesh, material, x, y, width, height, color);//GameObject(boxMesh, material);
	box->SetObjectPosition(position);
	box->UpdateWorldMatrix();
	UIObjects.push_back(box);
	boxMeshes.push_back(boxMesh);

	return box;
}

void UICanvas::CreateDebugFunction(wstring name, function<void(vector<wstring> parameters)> func) {
	debugWindow.CreateFunction(name, func);
}

void UICanvas::Enable(bool enabled) {
	this->enabled = enabled;
}

UIButton * UICanvas::CreateButton(float x, float y, float width, float height, Material *material, wstring text, XMFLOAT4 color) {
	XMFLOAT3 fwd = XMFLOAT3(0.0f, 0.0f, -1.0f);
	XMFLOAT2 uv[] = { XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f),XMFLOAT2(0.0f, 1.0f) };
	Vertex vertices[] = {
		{ XMFLOAT3(x - width / 2, y + height / 2, 0.0f), fwd, uv[0] },
	{ XMFLOAT3(x + width / 2, y + height / 2, 0.0f), fwd, uv[1] },
	{ XMFLOAT3(x + width / 2, y - height / 2, 0.0f), fwd, uv[2] },
	{ XMFLOAT3(x - width / 2, y - height / 2, 0.0f), fwd, uv[3] },
	};

	UINT indices[] = { 0, 1, 2, 0, 2, 3 };
	Mesh * boxMesh = new Mesh(&vertices[0], 4, &indices[0], 6, device);
	UIButton *box = new UIButton(boxMesh, material, x, y, width, height, text, color);//GameObject(boxMesh, material);
	box->SetObjectPosition(position);
	box->UpdateWorldMatrix();
	UIObjects.push_back(box);
	boxMeshes.push_back(boxMesh);
	return box;
}

UIRadioGroup * UICanvas::CreateRadioGroup(int id) {
	if (radioGroups[id] != nullptr)
		return radioGroups[id];
	UIRadioGroup *rg = new UIRadioGroup();
	rg->toggledButton = nullptr;
	//radioGroups.push_back(rg);
	//radioGroups.insert(pair<int, UIRadioGroup*>(id, rg));
	radioGroups[id] = rg;
	return rg;
}

bool UICanvas::DebugWindowActive() {
	return debugWindow.IsEnabled();
}

UIRadioButton * UICanvas::CreateRadioButton(float x, float y, float width, float height, XMFLOAT4 color, UIRadioGroup *radioGroup, Material *m) {
	XMFLOAT3 fwd = XMFLOAT3(0.0f, 0.0f, -1.0f);
	XMFLOAT2 uv[] = { XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f),XMFLOAT2(1.0f, 0.0f) };
	Vertex vertices[] = {
		{ XMFLOAT3(x - width / 2, y + height / 2, 0.0f), fwd, uv[0] },
	{ XMFLOAT3(x + width / 2, y + height / 2, 0.0f), fwd, uv[1] },
	{ XMFLOAT3(x + width / 2, y - height / 2, 0.0f), fwd, uv[2] },
	{ XMFLOAT3(x - width / 2, y - height / 2, 0.0f), fwd, uv[3] },
	};

	UINT indices[] = { 0, 1, 2, 0, 2, 3 };
	Mesh * boxMesh = new Mesh(&vertices[0], 4, &indices[0], 6, device);
	UIRadioButton *box = new UIRadioButton(boxMesh, m, x, y, width, height, color, radioGroup);//GameObject(boxMesh, material);
	box->SetObjectPosition(position);
	box->UpdateWorldMatrix();
	UIObjects.push_back(box);
	boxMeshes.push_back(boxMesh);
	return box;
}

void UICanvas::CreateBox(ID3D11DeviceContext* context, SimpleVertexShader* vs, SimplePixelShader* ps) {
	XMFLOAT3 fwd = XMFLOAT3(0.0f, 0.0f, -1.0f);
	XMFLOAT2 uv[] = { XMFLOAT2(0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f),XMFLOAT2(0.0f, 1.0f) };
	Vertex vertices[] = {
		{ XMFLOAT3(-3.5f, 1.0f, 0.0f), fwd, uv[0] },
	{ XMFLOAT3(3.5f, 1.0f, 0.0f), fwd, uv[1] },
	{ XMFLOAT3(3.5f, -1.0f, 0.0f), fwd, uv[2] },
	{ XMFLOAT3(-3.5f, -1.0f, 0.0f), fwd, uv[3] },
	};

	UINT indices[] = { 0, 1, 2, 0, 2, 3 };
	Mesh * boxMesh = new Mesh(&vertices[0], 4, &indices[0], 6, device);
	Material * material = new Material(vs, ps);
	//material->LoadTexture(L"Assets/Texture/texture01.jpg", device, context);
	UIObject *box = new UIObject(boxMesh, material, 0.0f, 0.0f, 0.0f, 0.0f, XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
	box->SetObjectPosition(position);
	box->UpdateWorldMatrix();
	UIObjects.push_back(box);
}

TextEntity * UICanvas::CreateText(wstring str, Alignment alignment, float x, float y) {
	TextEntity *newText = new TextEntity{ str, alignment, x, y };
	textEntities.push_back(newText);
	return newText;
}

/*void UICanvas::Prepare(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix) {
	for (int i = 0; i < GameObjects.size(); i++) {
		GameObjects[i].PrepareMaterial(viewMatrix, projectionMatrix);
	}
}*/

void UICanvas::Draw(Renderer *r) {
	if (!enabled) return;
	CommonStates states(device);
	spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());
	for (auto uiobj : UIObjects) {
		//r->Draw(uiobj, uiobj->GetColor(), true);
		//Make sure Materials and SRVs get initialized properly first
		if (uiobj->GetMaterial()->GetSRV() != nullptr)
		{
			float x = uiobj->GetX();
			float y = uiobj->GetY();
			x /= (5.0f * (0.25f * 3.1415926535f) * FACTOR);
			y /= (5.0f * (0.25f * 3.1415926535f) * FACTOR);
			y = -y;
			x += 640;
			y += 360;
			spriteBatch->Draw(uiobj->GetMaterial()->GetSRV(), Vector2(x, y), nullptr, uiobj->GetColor(), 0.0f, uiobj->GetPixelScale() / 2.0f, uiobj->GetScale() / uiobj->GetPixelScale() * 175, SpriteEffects::SpriteEffects_None, 0.0f);
		}
	}
	spriteBatch->End();

	//Reset the blend state and depth stencil state after drawing the Sprite Batch
	context->OMSetBlendState(0, 0, 0xFFFFFFFF);
	context->OMSetDepthStencilState(0, 0);
}

void UICanvas::DrawDebugWindow() {
	spriteFont->DrawString(spriteBatch.get(), debugWindow.GetCurrentLine().c_str(), Vector2(50.0f, 650.0f), Colors::White);
	int i = 0;
	for (auto it = debugWindow.GetLines()->begin(); it != debugWindow.GetLines()->end(); it++) {
		spriteFont->DrawString(spriteBatch.get(), (*it).text.c_str(), Vector2(50.0f, 650.0f - (20 * (MAX_LINES + 1)) + (20 * (i + 1))), (*it).color);
		i++;
	}
}

void UICanvas::DrawSprites() {
	//Draw objects with Sprite Batch
	//Draw the debug window whether the canvas is enabled or disabled
	spriteBatch->Begin();
	if (enabled) {
		for (auto uiobj : UIObjects) {
			UIButton * btn = dynamic_cast<UIButton*>(uiobj);
			if (btn != nullptr) {
				float x = btn->GetX();
				float y = btn->GetY();
				x /= (5.0f * (0.25f * 3.1415926535f) * FACTOR);
				y /= (5.0f * (0.25f * 3.1415926535f) * FACTOR);
				y = -y;
				x += 640;
				y += 360;
				Vector2 size = spriteFont->MeasureString(btn->GetText().c_str());
				spriteFont->DrawString(spriteBatch.get(), btn->GetText().c_str(), Vector2(x - (size.x / 2), y - (size.y / 2)), Colors::Black);
			}
		}
	}

		if (debugWindow.IsEnabled())
			DrawDebugWindow();

	if (enabled) {
		//Draw each text entity
		for (auto textEntity : textEntities) {
			Vector2 size = spriteFont->MeasureString(textEntity->Text.c_str());
			float x;
			switch (textEntity->Alignment) {
			case Alignment::Left:
				x = textEntity->x;
				break;

			case Alignment::Center:
				x = textEntity->x - (size.x / 2);
				break;

			case Alignment::Right:
				x = textEntity->x - size.x;
				break;
			}
			spriteFont->DrawString(spriteBatch.get(), textEntity->Text.c_str(), Vector2(x, textEntity->y), Colors::Red);
		}
	}

	spriteBatch->End();

	//Reset the blend state and depth stencil state after drawing the Sprite Batch
	context->OMSetBlendState(0, 0, 0xFFFFFFFF);
	context->OMSetDepthStencilState(0, 0);
}

void UICanvas::Update(float deltaTime, float totalTime) {
	for (auto uiobj : UIObjects) {
		uiobj->Update(deltaTime, totalTime);
	}
	debugWindow.Update(deltaTime);
}

void UICanvas::GetKeyInputString(wstring keyInput) {
	if (!debugWindow.IsEnabled()) return;
	if (keyInput != L"") {
		debugWindow.AddKey(keyInput);
	}
}

void UICanvas::EnterPressed() {
	if (!debugWindow.IsEnabled()) return;
	debugWindow.EnterPressed();
}

void UICanvas::BackspacePressed() {
	if (!debugWindow.IsEnabled()) return;
	debugWindow.BackspacePressed();
}

void UICanvas::GravePressed() {
	debugWindow.GravePressed();
}

void UICanvas::OnMouseMove(int screenX, int screenY, XMFLOAT4X4 projectionMatrix, XMFLOAT4X4 viewMatrix) {
	if (!enabled) return;
	float x = (screenX - 640.0f);
	float y = (screenY - 360.0f);

	x *= 5.0f * (0.25f * 3.1415926535f) * FACTOR;
	y *= 5.0f * (0.25f * 3.1415926535f) * FACTOR;
	y = -y;

	for (auto uiobj : UIObjects) {
		uiobj->OnMouseMove(x, y);
	}

	//std::cout << x << endl << y << endl << endl;
}

void UICanvas::OnMouseDown(int screenX, int screenY) {
	if (!enabled) return;
	float x = (screenX - 640.0f);
	float y = (screenY - 360.0f);

	x *= 5.0f * (0.25f * 3.1415926535f) * FACTOR;
	y *= 5.0f * (0.25f * 3.1415926535f) * FACTOR;
	y = -y;

	for (auto uiobj : UIObjects) {
		uiobj->OnMouseDown(x, y);
	}
}

void UICanvas::OnMouseUp() {
	if (!enabled) return;
	for (auto uiobj : UIObjects) {
		uiobj->OnMouseUp();
	}
}