//---------------------------------------------------------
// By Jishnu Girish
//---------------------------------------------------------
#include "GameObject.h"

using namespace DirectX;
GameObject::GameObject()
{
}

//-------------------------------------------------------------------
// Initialize GameObjects with a mesh and material
//-------------------------------------------------------------------
GameObject::GameObject(Mesh * mesh,Material* mat, bool box, bool sph)
{
	objectMesh = mesh;
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&objectWorldMatrix, XMMatrixTranspose(W));

	position = XMFLOAT3(0, 0, 0);
	rotation = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);

	// Assign a material to the game object
	material = mat;

	soundManager = nullptr;

	if (mesh != nullptr)
	{
		hasBox = box;
		hasSphere = sph;

		auto minV = mesh->GetMinVertex();
		auto maxV = mesh->GetMaxVertex();
		XMFLOAT3 center = XMFLOAT3((maxV.x - minV.x) / 2, (maxV.y - minV.y) / 2, (maxV.z - minV.z) / 2);
		boundingBox.Extents = center;
		auto actualCenter = XMVectorAdd(XMLoadFloat3(&center), XMLoadFloat3(&position));
		XMStoreFloat3(&center, actualCenter);
		boundingBox.Center = position;

		boundingSphere.Center = center;
		_radius = (maxV.x - minV.x) / 2;
		boundingSphere.Radius = _radius;
	}
}

//------------------------------------------------------------------------------
// Initialize GameObjects with a mesh, material and attach sounds to the object
//------------------------------------------------------------------------------
GameObject::GameObject(Mesh * mesh, Material* mat, std::vector<std::string> soundfiles, AudioEngine *sManager, bool box, bool sph)
{
	objectMesh = mesh;
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&objectWorldMatrix, XMMatrixTranspose(W));

	position = XMFLOAT3(0, 0, 0);
	rotation = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);

	// Assign a material to the game object
	material = mat;

	sounds = soundfiles;
	soundManager = sManager;
	LoadSound();
	if (mesh != nullptr)
	{
		hasBox = box;
		hasSphere = sph;

		auto minV = mesh->GetMinVertex();
		auto maxV = mesh->GetMaxVertex();
		XMFLOAT3 center = XMFLOAT3((maxV.x - minV.x) / 2, (maxV.y - minV.y) / 2, (maxV.z - minV.z) / 2);
		boundingBox.Extents = center;
		auto actualCenter = XMVectorAdd(XMLoadFloat3(&center), XMLoadFloat3(&position));
		XMStoreFloat3(&center, actualCenter);
		boundingBox.Center = position;

		boundingSphere.Center = center;
		_radius = (maxV.x - minV.x) / 2;
		boundingSphere.Radius = _radius;
	}
}

GameObject::~GameObject()
{
	delete bBoxMesh;
}

//-------------------------------------------------------------------
// Set the world matrix of the gameobject
//-------------------------------------------------------------------
void GameObject::SetObjectWorldMatrix(DirectX::XMMATRIX newWorldMatrix)
{
	XMStoreFloat4x4(&objectWorldMatrix, XMMatrixTranspose(newWorldMatrix));		// Transpose is stored to be used in shader
}

//-------------------------------------------------------------------
// Set position of game object
//-------------------------------------------------------------------
void GameObject::SetObjectPosition(DirectX::XMFLOAT3 newPosition )
{
	position = newPosition;
	boundingBox.Center = newPosition;
}

//-------------------------------------------------------------------
// Set rotation of the game object
//-------------------------------------------------------------------
void GameObject::SetObjectRotation(DirectX::XMFLOAT3 newRotation)
{
	rotation = newRotation;
}

//-------------------------------------------------------------------
// Set the scale of the gameobject
//-------------------------------------------------------------------
void GameObject::SetObjectScale(DirectX::XMFLOAT3 newScale)
{
	scale = newScale;
}

//-------------------------------------------------------------------
// Return the world matrix
//-------------------------------------------------------------------
DirectX::XMFLOAT4X4 GameObject::GetObjectWorldMatrix()
{
	return objectWorldMatrix;
}

//-------------------------------------------------------------------
// Return the object position
//-------------------------------------------------------------------
DirectX::XMFLOAT3 GameObject::GetObjectPosition()
{
	return position;
}

//-------------------------------------------------------------------
// Return the object rotation
//-------------------------------------------------------------------
DirectX::XMFLOAT3 GameObject::GetObjectRotation()
{
	return rotation;
}

//-------------------------------------------------------------------
// Return the scale of the game object
//-------------------------------------------------------------------
DirectX::XMFLOAT3 GameObject::GetObjectScale()
{
	return scale;
}

//-------------------------------------------------------------------
// Return the mesh of the game object
//-------------------------------------------------------------------
Mesh* GameObject::GetObjectMesh()
{
	return objectMesh;
}

//-------------------------------------------------------------------
// Update the world matrix values of the gameobject
//-------------------------------------------------------------------
void GameObject::UpdateWorldMatrix()
{
	GetBoundingBox();
	GetBoundingSphere();
	XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX rotMatrix = XMMatrixRotationRollPitchYaw(rotation.x,rotation.y,rotation.z);
	XMMATRIX translateMatrix = XMMatrixTranslation(position.x, position.y, position.z);
	XMStoreFloat4x4(&objectWorldMatrix, XMMatrixTranspose(scaleMatrix * rotMatrix * translateMatrix));
	for (auto channel : soundChannels)
	{
		soundManager->SetChannel3DPosition(channel, AudioVector3{position.x,position.y,position.z});
	}
}

//-------------------------------------------------------------------
// Move the game object with given velocity
//-------------------------------------------------------------------
void GameObject::Move(XMVECTOR velocityVector)
{
	XMVECTOR pos = XMVectorSet(position.x, position.y, position.z, 0);
	pos += velocityVector;
	XMStoreFloat3(&position, pos);
	UpdateWorldMatrix();
}

//-------------------------------------------------------------------
// Move object according to individual values
// Overloaded for abstarction
//-------------------------------------------------------------------
void GameObject::Move(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
	UpdateWorldMatrix();
}

//-------------------------------------------------------------------
// Rotate game object
//-------------------------------------------------------------------
void GameObject::Rotate(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
	UpdateWorldMatrix();
}

//----------------------------------------------------------------------
// Set the material shader values with right textures and shader values
//----------------------------------------------------------------------
void GameObject::PrepareMaterial(DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix, bool bBoxOn, bool bSphereOn)
{
	this->material->GetVertexShader()->SetMatrix4x4("view", viewMatrix);
	this->material->GetVertexShader()->SetMatrix4x4("projection", projectionMatrix);
	if(!bBoxOn && !bSphereOn)
		this->material->GetVertexShader()->SetMatrix4x4("world", this->objectWorldMatrix);
	else if(bBoxOn)
		this->material->GetVertexShader()->SetMatrix4x4("world", this->bBoxWorldMatrix);
	else if (bSphereOn)
		this->material->GetVertexShader()->SetMatrix4x4("world", this->bSphereWorldMatrix);
	this->material->GetVertexShader()->SetShader();

	this->material->GetPixelShader()->SetShaderResourceView("DiffuseTexture", this->material->GetSRV());
	
	if(this->material->GetSampler() != nullptr)
		this->material->GetPixelShader()->SetSamplerState("Sampler", this->material->GetSampler());
	
	if (material->GetNormalSRV())
		this->material->GetPixelShader()->SetShaderResourceView("NormalTexture", material->GetNormalSRV());
	else
		this->material->GetPixelShader()->SetShaderResourceView("NormalTexture", nullptr);
	
	this->material->GetPixelShader()->SetShader();

	this->material->GetVertexShader()->CopyAllBufferData();
	this->material->GetPixelShader()->CopyAllBufferData();
}

//-------------------------------------------------------------------
// Return the material of the object
//-------------------------------------------------------------------
Material* GameObject::GetMaterial()
{
	return material;
}

//-------------------------------------------------------------------
// Load Sound from sounds vector
//-------------------------------------------------------------------
void GameObject::LoadSound()
{
	if (!sounds.empty())
	{
		for (auto sound : sounds)
		{
			soundManager->LoadSound(sound, true, true);
		}
	}
}

//-------------------------------------------------------------------
// Play Sound
// Id is the sound id in the sounds vector
// fadeIn is the time taken to fade the sound to max volume in seconds
//-------------------------------------------------------------------
void GameObject::PlayObjectSound(int Id, float fadeIn)
{
	if(!sounds.empty() && Id < sounds.size() && !(Id < soundChannels.size()) )	// Checks if the sound is playing currently
		soundChannels.push_back(soundManager->PlaySounds(sounds[Id], AudioVector3{ position.x,position.y,position.z }, soundManager->VolumeToDecibal(1.0f), fadeIn));
}

//---------------------------------------------------------------------
// Stop Sound
// Id is the sound id in the sound channels vector
// fadeIn is the time taken to fade the sound to zero volume in seconds
//---------------------------------------------------------------------
void GameObject::StopObjectSound(int Id, float fadeOut)
{
	if (!soundChannels.empty())
	{
		soundManager->StopChannel(soundChannels[Id], fadeOut);
		soundChannels.erase(soundChannels.begin() + Id);		// removes the sound that is not playing from channel list
	}
}

//-------------------------------------------------------------------
// Return the bounding box
//-------------------------------------------------------------------
BoundingOrientedBox GameObject::GetBoundingBox()
{
	auto bBox = boundingBox;
	bBox.Center = position;
	bBox.Extents.x *= scale.x ;
	bBox.Extents.y *= scale.y ;
	bBox.Extents.z *= scale.z ;
	auto rot = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));
	XMStoreFloat4(&bBox.Orientation, rot);
	boundingBox = bBox;
	XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);									// Calculate new scale matrix
	XMMATRIX rotMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&bBox.Orientation));					// Calculate new rotation matrix
	XMMATRIX translateMatrix = XMMatrixTranslation(bBox.Center.x, bBox.Center.y, bBox.Center.z);		// Calculate new position
	XMStoreFloat4x4(&bBoxWorldMatrix, XMMatrixTranspose(scaleMatrix * rotMatrix * translateMatrix));	// Calculate world matrix
	return bBox;
}

//-------------------------------------------------------------------
// Return the bounding sphere
//-------------------------------------------------------------------
BoundingSphere GameObject::GetBoundingSphere()
{
	boundingSphere.Center = position;
	boundingSphere.Radius *= scale.x;
	XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);									// Calculate new scale matrix
	XMMATRIX rotMatrix = XMMatrixRotationRollPitchYaw(0, 0, 0);					// Calculate new rotation matrix
	XMMATRIX translateMatrix = XMMatrixTranslation(0, 0, 0);		// Calculate new position
	XMStoreFloat4x4(&bSphereWorldMatrix, XMMatrixTranspose(scaleMatrix * rotMatrix * translateMatrix));	// Calculate world matrix
	return boundingSphere;
}

//-------------------------------------------------------------------
// Set the bounding box mesh
//-------------------------------------------------------------------
Mesh* GameObject::SetBboxMesh(ID3D11Device * device)
{
	if (hasBox)
	{
		std::vector<Vertex> verts;												// Set up vertex array
		XMFLOAT3 corners[8];													// Container for bBox corners
		BoundingOrientedBox b = GetBoundingBox();
		boundingBox.GetCorners(corners);										// Get Bbox corners

		// Convert from bBox positions to custom vertex
		for (int i = 0; i < 8; i++)
		{
			Vertex v;
			v.Position = XMFLOAT3(corners[i].x, corners[i].y, corners[i].z);
			v.Normal = XMFLOAT3(0.0, 0.0, 0.0);
			v.Tangent = XMFLOAT3(0.0, 0.0, 0.0);
			v.UV = XMFLOAT2(0.0, 0.0);
			verts.push_back(v);
		}

		// Setup index buffer to darw bBox
		std::vector<unsigned int> indices =
		{
			0, 1,
			1, 2,
			2, 3,
			3, 0,
			4, 5,
			5, 6,
			6, 7,
			7, 4,
			0, 4,
			1, 5,
			2, 6,
			3, 7
		};
		bBoxMesh = new Mesh(verts.data(), 8, indices.data(), indices.size(), device);

		return bBoxMesh;
	}
	else
		return nullptr;
}

//-------------------------------------------------------------------
// Return bounding box mesh
//-------------------------------------------------------------------
Mesh* GameObject::GetBboxMesh()
{
	return bBoxMesh;
}

//-------------------------------------------------------------------
// Return true if object has bounding box
//-------------------------------------------------------------------
bool GameObject::hasBbox()
{
	return hasBox;
}

//-------------------------------------------------------------------
// Return true if object has bounding sphere
//-------------------------------------------------------------------
bool GameObject::hasBsphere()
{
	return hasSphere;
}
