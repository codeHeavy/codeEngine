//Portrait.h
//By Joshua Rand

#pragma once

#include <d3d11.h>

class Portrait {
public:
	Portrait(ID3D11ShaderResourceView *srv, int x = 0, int y = 0);
	void SetPosition(int x, int y);
	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();
	ID3D11ShaderResourceView * GetSRV();
private:
	int x;
	int y;
	int width;
	int height;
	ID3D11ShaderResourceView *srv;
	void CalculateWidthAndHeight();
};