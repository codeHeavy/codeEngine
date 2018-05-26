//-----------------------------------------------------
// By Jishnu Girish
//-----------------------------------------------------
#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "UserInputHandler.h"
class ThirdPersonObject: public GameObject
{
private:
	float rotSpeed;
	float speed;
	DirectX::XMVECTOR forwardDir;
	Camera * cam;
	int life;
	bool intersecting;
public:
	void SetCamera(Camera* _cam);
	void Update(float deltaTime);
	ThirdPersonObject(Mesh* mesh, Material *mat, Camera* cam, bool box = false, bool sph = false);
	void SetIntersecting(bool intersecting);
	bool GetIntersecting();
	void TakeDamage();
	int GetLife();
	~ThirdPersonObject();
};

