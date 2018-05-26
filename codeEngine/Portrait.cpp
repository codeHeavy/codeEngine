//Portrait.h
//By Joshua Rand

#include "Portrait.h"

Portrait::Portrait(ID3D11ShaderResourceView *srv, int x, int y) {
	this->srv = srv;
	this->x = x;
	this->y = y;

	CalculateWidthAndHeight();
}

void Portrait::SetPosition(int x, int y) {
	this->x = x;
	this->y = y;
}

int Portrait::GetX() {
	return x;
}

int Portrait::GetY() {
	return y;
}

int Portrait::GetWidth() {
	return width;
}

int Portrait::GetHeight() {
	return height;
}

ID3D11ShaderResourceView * Portrait::GetSRV() {
	return srv;
}

//A roundabout way of simply getting the width from a ShaderResourceView
//Convert the SRV to a Texture2D description
//It's important to calculate the width ONLY when necessary, because converting it to a texture every time the width or height is requested will waste memory
//It's important to use reference parameters and not pointers
void Portrait::CalculateWidthAndHeight() {
	ID3D11Resource * rs = nullptr;
	D3D11_TEXTURE2D_DESC desc;

	srv->GetResource(&rs);
	ID3D11Texture2D *texture = (ID3D11Texture2D*)(rs);
	texture->GetDesc(&desc);

	width = desc.Width;
	height = desc.Height;
	texture->Release();
}