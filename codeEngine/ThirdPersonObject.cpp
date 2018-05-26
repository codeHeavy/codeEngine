//-----------------------------------------------------
// By Jishnu Girish
//-----------------------------------------------------
#include "ThirdPersonObject.h"

//-----------------------------------------------------
// Initialize 3rd person object
//-----------------------------------------------------
ThirdPersonObject::ThirdPersonObject(Mesh* mesh, Material *mat, Camera* _cam,bool box,bool sph) : GameObject(mesh, mat,box,sph)
{
	speed = 5.0f;
	rotSpeed = 0.5f;
	cam = _cam;
	intersecting = false;
	life = 4;
}


ThirdPersonObject::~ThirdPersonObject()
{
}

//-----------------------------------------------------
// Set third person camera instance
//-----------------------------------------------------
void ThirdPersonObject::SetCamera(Camera* _cam)
{
	cam = _cam;
}

//-----------------------------------------------------
// Update thrid person object and 3rd person camera
//-----------------------------------------------------
void ThirdPersonObject::Update(float deltaTime)
{
	auto pos = XMLoadFloat3(&position);
	forwardDir = -XMVector3Rotate(XMVectorSet(0, 0, 1, 0), XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation)));
	XMVECTOR up = XMVectorSet(0, 1, 0, 0); // Y is up!
	
	if (KeyInput::IsKeyDown('W'))
	{
		pos = pos + forwardDir * speed * deltaTime;
	}

	if (KeyInput::IsKeyDown('S'))
	{
		pos = pos - forwardDir * speed * deltaTime;
	}

	if (KeyInput::IsKeyDown('A'))
	{
		Rotate(0, -rotSpeed * deltaTime, 0);
	}

	if (KeyInput::IsKeyDown('D'))
	{
		Rotate(0, rotSpeed * deltaTime, 0);
	}
	XMStoreFloat3(&position, pos);
	if (cam->GetType() == ThirdPersonCam)
	{
		cam->UpdateViewMatrix3P(position,rotation,forwardDir);
	}
	cam->SetSpeed(speed);
	UpdateWorldMatrix();
}

void ThirdPersonObject::SetIntersecting(bool intersecting) {
	this->intersecting = intersecting;
}

bool ThirdPersonObject::GetIntersecting() {
	return intersecting;
}

void ThirdPersonObject::TakeDamage() {
	if (life > 0)
		life--;
}

int ThirdPersonObject::GetLife() {
	return life;
}