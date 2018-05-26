//---------------------------------------------------------
// By Jishnu Girish
//---------------------------------------------------------
#include "Renderer.h"

//-------------------------------------------------------------------
// Initialize Renderer
//-------------------------------------------------------------------
Renderer::Renderer(ID3D11DepthStencilView * _depthStencilView, ID3D11RenderTargetView *_backBufferRTV,
	ID3D11DeviceContext *_context, IDXGISwapChain *_swapChain,
	Camera *_camera, ID3D11Device *device)
{
	depthStencilView = _depthStencilView;
	backBufferRTV = _backBufferRTV;
	context = _context;
	swapChain = _swapChain;
	camera = _camera;

	viewMatrix = camera->GetViewMatrix();

	//Create rasterizer state for wireframe
	D3D11_RASTERIZER_DESC rs = {};
	rs.CullMode = D3D11_CULL_NONE;
	rs.FillMode = D3D11_FILL_WIREFRAME;
	//Create a rasterizer state to draw the wireframes of objects
	device->CreateRasterizerState(&rs, &wireframeRastState);
	wireframeMode = false;

	batch = std::make_unique<PrimitiveBatch<Vertex>>(context);
}


Renderer::~Renderer()
{
	wireframeRastState->Release();
}

//-------------------------------------------------------------------
// Draw a single game object
//-------------------------------------------------------------------
void Renderer::Draw(GameObject * gameObject, XMFLOAT4 color, bool onCanvas)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* vBuff;
	ID3D11Buffer* iBuff;



	gameObject->GetMaterial()->GetPixelShader()->SetFloat3("CameraPosition", camera->GetPosition());
	gameObject->GetMaterial()->GetPixelShader()->SetFloat4("color", color);
	if (onCanvas) {
		gameObject->GetMaterial()->GetPixelShader()->SetInt("bBox", 0);
		gameObject->GetMaterial()->GetVertexShader()->SetInt("onCanvas", 1);
		gameObject->GetMaterial()->GetPixelShader()->SetInt("onCanvas", 1);
		gameObject->PrepareMaterial(viewMatrix, camera->GetProjectionMatrix());
	}
	else {
		gameObject->GetMaterial()->GetPixelShader()->SetInt("bBox", 0);
		gameObject->GetMaterial()->GetVertexShader()->SetInt("onCanvas", 0);
		gameObject->GetMaterial()->GetPixelShader()->SetInt("onCanvas", 0);
		gameObject->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix());
		//Draw object wireframes if wireframe mode is active
		//Only draw objects that are not on the canvas in wireframe
		if (wireframeMode)
			context->RSSetState(wireframeRastState);// Set rasterizer state
		else
			context->RSSetState(0);// Set rasterizer state
	}
	vBuff = gameObject->GetObjectMesh()->GetVertexBuffer();
	iBuff = gameObject->GetObjectMesh()->GetIndexBuffer();
	context->IASetVertexBuffers(0, 1, &vBuff, &stride, &offset);
	context->IASetIndexBuffer(iBuff, DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(
		gameObject->GetObjectMesh()->GetIndexCount(),   // The number of indices to use
		0,												// Offset to the first index we want to use
		0);												// Offset to add to each index when looking up vertices
	context->RSSetState(0);
}

void Renderer::Draw(GameObject * gameObject, XMFLOAT4 color) {
	Draw(gameObject, color, false);
}

void Renderer::Draw(GameObject * gameObject) {
	Draw(gameObject, XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), false);
}

//-------------------------------------------------------------------
// Draw a bounding box
//-------------------------------------------------------------------
void Renderer::DrawLines(GameObject * gameObject,ID3D11Device* device, XMFLOAT4 color)
{
	if (gameObject->hasBbox())
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		ID3D11Buffer* vBuff;
		ID3D11Buffer* iBuff;
		//auto color = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
		gameObject->GetMaterial()->GetPixelShader()->SetFloat3("CameraPosition", camera->GetPosition());
		gameObject->GetMaterial()->GetPixelShader()->SetFloat4("color", color);
		gameObject->GetMaterial()->GetPixelShader()->SetInt("bBox", 1);
		gameObject->GetMaterial()->GetVertexShader()->SetInt("onCanvas", 0);
		gameObject->GetMaterial()->GetPixelShader()->SetInt("onCanvas", 0);
		//Draw object wireframes if wireframe mode is active
		//Only draw objects that are not on the canvas in wireframe
		if (wireframeMode)
			context->RSSetState(wireframeRastState);// Set rasterizer state
		else
			context->RSSetState(0);// Set rasterizer state
		auto bBoxMesh = gameObject->GetBboxMesh();
		gameObject->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix(), true);
		vBuff = bBoxMesh->GetVertexBuffer();
		iBuff = bBoxMesh->GetIndexBuffer();
		context->IASetVertexBuffers(0, 1, &vBuff, &stride, &offset);
		context->IASetIndexBuffer(iBuff, DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST); //Set to Line list
		context->DrawIndexed((UINT)bBoxMesh->GetIndexCount(), 0, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //Reset to triangle list
	}
}

//-------------------------------------------------------------------
// Clear the screen
//-------------------------------------------------------------------
void Renderer::Clear(const float color[4])
{
	// Clear the render target and depth buffer (erases what's on the screen)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);
}

//-------------------------------------------------------------------
// Present the swap chain to the buffer for viewing
//-------------------------------------------------------------------
void Renderer::PresentSwapChain()
{
	swapChain->Present(0, 0);
}

//-------------------------------------------------------------------
// Render skybox
//-------------------------------------------------------------------
void Renderer::RenderSky(Mesh *skymesh, SimplePixelShader *skyBoxPS, SimpleVertexShader *skyBoxVS, ID3D11SamplerState* sampler,
ID3D11ShaderResourceView* skySRV,ID3D11RasterizerState* skyRastState,ID3D11DepthStencilState* skyDepthState)
{
	// Set buffers in the input assembler
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	// Draw the sky
	ID3D11Buffer* skyVB = skymesh->GetVertexBuffer();
	ID3D11Buffer* skyIB = skymesh->GetIndexBuffer();
	context->IASetVertexBuffers(0, 1, &skyVB, &stride, &offset);
	context->IASetIndexBuffer(skyIB, DXGI_FORMAT_R32_UINT, 0);

	// Set up the sky shaders
	skyBoxVS->SetMatrix4x4("view", camera->GetViewMatrix());
	skyBoxVS->SetMatrix4x4("projection", camera->GetProjectionMatrix());
	skyBoxVS->CopyAllBufferData();
	skyBoxVS->SetShader();

	skyBoxPS->SetShaderResourceView("SkyTexture", skySRV);
	skyBoxPS->SetSamplerState("BasicSampler", sampler);
	skyBoxPS->SetShader();

	// Set up the render states necessary for the sky
	context->RSSetState(skyRastState);
	context->OMSetDepthStencilState(skyDepthState, 0);
	context->DrawIndexed(skymesh->GetIndexCount(), 0, 0);

	// When done rendering, reset any and all states for the next frame
	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);
}

void Renderer::SetDepthStencilView(ID3D11DepthStencilView * _depthStencilView)
{
	depthStencilView = _depthStencilView;
}

void Renderer::SetBackBuffer(ID3D11RenderTargetView * _backBufferRTV)
{
	backBufferRTV = _backBufferRTV;
}

//-------------------------------------------------------------------
// Toggle wireframe mode on or off
//-------------------------------------------------------------------
void Renderer::SetWireframeMode(bool set) {
	wireframeMode = set;
}

//-------------------------------------------------------------------
// Toggle wireframe mode on or off
//-------------------------------------------------------------------
void Renderer::ToggleWireframeMode() {
	wireframeMode = !wireframeMode;
}

//-------------------------------------------------------------------
// Draw a bounding sphere of a gameobject if it exists
//-------------------------------------------------------------------
void Renderer::DrawSphere(GameObject* gameObject, XMFLOAT4 color)
{
	if (gameObject->hasBsphere())
	{
		// Setup shaders
		gameObject->GetMaterial()->GetPixelShader()->SetFloat3("CameraPosition", camera->GetPosition());
		gameObject->GetMaterial()->GetPixelShader()->SetFloat4("color", color);
		gameObject->GetMaterial()->GetPixelShader()->SetInt("bBox", 1);
		gameObject->GetMaterial()->GetVertexShader()->SetInt("onCanvas", 0);
		gameObject->GetMaterial()->GetPixelShader()->SetInt("onCanvas", 0);
		//Draw object wireframes if wireframe mode is active
		//Only draw objects that are not on the canvas in wireframe
		if (wireframeMode)
			context->RSSetState(wireframeRastState);// Set rasterizer state
		else
			context->RSSetState(0);// Set rasterizer state
		auto bBoxMesh = gameObject->GetBboxMesh();
		gameObject->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix(), false, true);

		// Setup drawing
		auto sphere = gameObject->GetBoundingSphere();
		XMVECTOR origin = XMLoadFloat3(&sphere.Center);

		const float radius = sphere.Radius;

		XMVECTOR xaxis = g_XMIdentityR0 * radius;
		XMVECTOR yaxis = g_XMIdentityR1 * radius;
		XMVECTOR zaxis = g_XMIdentityR2 * radius;
		batch->Begin();
		DrawRing(origin, xaxis, zaxis);
		DrawRing(origin, xaxis, yaxis);
		DrawRing(origin, yaxis, zaxis);
		batch->End();
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //Reset to triangle list
	}
}

//-------------------------------------------------------------------
// Draw a ring along the given axies
//-------------------------------------------------------------------
void Renderer::DrawRing(FXMVECTOR origin, FXMVECTOR majorAxis, FXMVECTOR minorAxis)
{
	
	static const size_t c_ringSegments = 32;

	Vertex verts[c_ringSegments + 1];

	FLOAT fAngleDelta = XM_2PI / float(c_ringSegments);

	XMVECTOR cosDelta = XMVectorReplicate(cosf(fAngleDelta));
	XMVECTOR sinDelta = XMVectorReplicate(sinf(fAngleDelta));
	XMVECTOR incrementalSin = XMVectorZero();
	static const XMVECTORF32 s_initialCos =
	{
		1.f, 1.f, 1.f, 1.f
	};
	XMVECTOR incrementalCos = s_initialCos.v;
	for (size_t i = 0; i < c_ringSegments; i++)
	{
		XMVECTOR pos = XMVectorMultiplyAdd(majorAxis, incrementalCos, origin);
		pos = XMVectorMultiplyAdd(minorAxis, incrementalSin, pos);
		XMStoreFloat3(&verts[i].Position, pos);
		
		// formula to rotate a vector.
		XMVECTOR newCos = incrementalCos * cosDelta - incrementalSin * sinDelta;
		XMVECTOR newSin = incrementalCos * sinDelta + incrementalSin * cosDelta;
		incrementalCos = newCos;
		incrementalSin = newSin;
	}
	verts[c_ringSegments] = verts[0];

	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, verts, c_ringSegments + 1);
}