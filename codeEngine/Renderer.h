#pragma once

#include <d3d11.h>
#include "Camera.h"
#include "Lights.h"
#include "GameObject.h"
#include <PrimitiveBatch.h>
class Renderer
{
private:
	ID3D11DepthStencilView * depthStencilView;
	ID3D11RenderTargetView *backBufferRTV;
	ID3D11DeviceContext *context;
	IDXGISwapChain *swapChain;
	Camera *camera;
	XMFLOAT4X4 viewMatrix;
	ID3D11RasterizerState* wireframeRastState;
	bool wireframeMode;
	std::unique_ptr<PrimitiveBatch<Vertex>> batch;
	void DrawRing(FXMVECTOR origin, FXMVECTOR majorAxis, FXMVECTOR minorAxis);
public:
	Renderer(ID3D11DepthStencilView * depthStencilView,
	ID3D11RenderTargetView *backBufferRTV,
	ID3D11DeviceContext *context,
	IDXGISwapChain *swapChain,
	Camera *camera, ID3D11Device *device);
	void Draw(GameObject * gameobject, XMFLOAT4 color, bool onCanvas);
	void Draw(GameObject * gameobject, XMFLOAT4 color);
	void Draw(GameObject * gameobject);
	void DrawLines(GameObject * gameObject, ID3D11Device* device, DirectX::XMFLOAT4 color = XMFLOAT4(0.0f,0.0f,1.0f,0.0f));
	void DrawSphere( GameObject* gameObject, XMFLOAT4 color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f));
	void Clear(const float color[4]);
	void PresentSwapChain();
	void RenderSky(Mesh *skymesh, SimplePixelShader *SkyPS, SimpleVertexShader *SkyVS, ID3D11SamplerState* sampler,
		ID3D11ShaderResourceView* skySRV, ID3D11RasterizerState* skyRastState, ID3D11DepthStencilState* skyDepthState);
	void SetDepthStencilView(ID3D11DepthStencilView * _depthStencilView);
	void SetBackBuffer(ID3D11RenderTargetView * _backBufferRTV);
	void SetWireframeMode(bool set);
	void ToggleWireframeMode();
	~Renderer();

};

