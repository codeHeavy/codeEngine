#pragma once

#include <d3d11.h>
#include "Vertex.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <DirectXMath.h>
using namespace DirectX;
class Mesh
{
private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int numOfIndices;
	XMFLOAT3 minVertex;
	XMFLOAT3 maxVertex;
	XMFLOAT3 minX;
	XMFLOAT3 minY;
	XMFLOAT3 minZ;
	XMFLOAT3 maxX;
	XMFLOAT3 maxY;
	XMFLOAT3 maxZ;
public:
	Mesh();
	Mesh(Vertex* vertexArray, int numOfVertices, unsigned int* indexArray, int numOfIndices, ID3D11Device* device);
	Mesh(const char* fileName, ID3D11Device* device);
	~Mesh();
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();
	void CreateBuffers(Vertex* vertexArray, int numOfVertices, unsigned int* indexArray, int numOfIndices, ID3D11Device* device);
	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
	XMFLOAT3 GetMinVertex() const;
	XMFLOAT3 GetMaxVertex() const;
	XMFLOAT3 modelMax;
	XMFLOAT3 modelMin;

	void CalculateBoundingBox();
};

