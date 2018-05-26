//---------------------------------------------------------
// By Jishnu Girish
//---------------------------------------------------------
#include "Camera.h"
#include "UserInputHandler.h"
using namespace DirectX;

//-------------------------------------------------------------------
// Default camera Initialization
//-------------------------------------------------------------------
Camera::Camera()
{
	position = DirectX::XMFLOAT3(0,0,-10);
	direction = DirectX::XMFLOAT3(0, 0, 1);
	upDirection = DirectX::XMFLOAT3(0, 1, 0);
	XMStoreFloat4(&rotation, XMQuaternionIdentity());
	XMStoreFloat3(&newUp, XMVectorSet(0,1,0,0));

	rotationX = 0;
	rotationY = 0;

	speed = 3;
	camSensitivity = (float)0.005;
	type = FreeFormCam;

	XMStoreFloat4x4(&viewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&projectionMatrix, XMMatrixIdentity());
}

//-------------------------------------------------------------------
// Initialize cameara at a given position
//-------------------------------------------------------------------
Camera::Camera(float x, float y, float z)
{
	position = DirectX::XMFLOAT3(x, y, z);
	direction = DirectX::XMFLOAT3(0, 0, 1);
	upDirection = DirectX::XMFLOAT3(0, 1, 0);
	XMStoreFloat4(&rotation, XMQuaternionIdentity());
	XMStoreFloat3(&newUp, XMVectorSet(0, 1, 0, 0));

	rotationX = 0;
	rotationY = 0;

	speed = 25;
	camSensitivity = (float)0.005;
	type = FreeFormCam;

	XMStoreFloat4x4(&viewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&projectionMatrix, XMMatrixIdentity());
}

Camera::~Camera()
{
}

//-------------------------------------------------------------------
// Return projection matrix
//-------------------------------------------------------------------
DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

//-------------------------------------------------------------------
// set camera position
//-------------------------------------------------------------------
void Camera::SetPosition(DirectX::XMFLOAT3 pos)
{
	position = pos;
}

//-------------------------------------------------------------------
// Return camera position
//-------------------------------------------------------------------
DirectX::XMFLOAT3 Camera::GetPosition()
{
	return position;
}

//-------------------------------------------------------------------
// Set forward direction
//-------------------------------------------------------------------
void Camera::SetDirection(DirectX::XMFLOAT3 dir)
{
	direction = dir;
}

//-------------------------------------------------------------------
// Return forward direction
//-------------------------------------------------------------------
DirectX::XMFLOAT3 Camera::GetDirection()
{
	return direction;
}

//-------------------------------------------------------------------
// Return current up direction
//-------------------------------------------------------------------
DirectX::XMFLOAT3 Camera::GetUp()
{
	return newUp;
}
//-------------------------------------------------------------------
// Set X rotation
//-------------------------------------------------------------------
void Camera::SetRotationX(float rotX)
{
	float rotation = rotX ;
	
	rotationX += rotation;
}

//-------------------------------------------------------------------
// Set Y rotation
//-------------------------------------------------------------------
void Camera::SetRotationY(float rotY)
{
	float rotation = rotY ;

	rotationY += rotation;
}

//-------------------------------------------------------------------
// Return X rotation
//-------------------------------------------------------------------
float Camera::GetRotationX()
{
	return rotationX;
}

//-------------------------------------------------------------------
// Return Y rotation
//-------------------------------------------------------------------
float Camera::GetRotationY()
{
	return rotationY;
}

//-------------------------------------------------------------------
// Rotate camera with current rotation values
//-------------------------------------------------------------------
void Camera::Rotate()
{
	DirectX::XMVECTOR rotVector = DirectX::XMQuaternionRotationRollPitchYaw(rotationY,rotationX,0);		// rotX is rotation along Y-axis
	DirectX::XMStoreFloat4(&rotation, rotVector);
}

//-------------------------------------------------------------------
// Moves the camera relative to its orientation
//-------------------------------------------------------------------
void Camera::MoveRelative(float x, float y, float z)
{
	// Rotate the desired movement vector
	XMVECTOR dir = XMVector3Rotate(XMVectorSet(x, y, z, 0), XMLoadFloat4(&rotation));

	// Move in that direction
	XMStoreFloat3(&position, XMLoadFloat3(&position) + dir);
}

//-------------------------------------------------------------------
// Moves the camera in world space (not local space)
//-------------------------------------------------------------------
void Camera::MoveAbsolute(float x, float y, float z)
{
	// Simple add, no need to load/store
	position.x += x;
	position.y += y;
	position.z += z;
}

//-------------------------------------------------------------------
// Update camera from user Input
//-------------------------------------------------------------------
void Camera::UpdateFromInput(float deltatime)
{
	if (type == FreeFormCam) 
	{
		// Movement
		if (KeyInput::IsKeyDown('W')) { MoveRelative(0, 0, speed * deltatime); }
		if (KeyInput::IsKeyDown('S')) { MoveRelative(0, 0, -speed * deltatime); }
		if (KeyInput::IsKeyDown('A')) { MoveRelative(-speed * deltatime, 0, 0); }
		if (KeyInput::IsKeyDown('D')) { MoveRelative(speed * deltatime, 0, 0); }
		if (KeyInput::IsKeyDown('X')) { MoveAbsolute(0, -speed * deltatime, 0); }
		if (KeyInput::IsKeyDown(VK_SPACE)) { MoveAbsolute(0, speed * deltatime, 0); }
		UpdateViewMatrix();
	}
}

//-------------------------------------------------------------------
// Update View Matrix
//-------------------------------------------------------------------
void Camera::UpdateViewMatrix()
{
	XMVECTOR dir = XMVector3Rotate(XMVectorSet(0, 0, 1, 0), XMLoadFloat4(&rotation));
	XMVECTOR up = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), XMLoadFloat4(&rotation));
	DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(
		DirectX::XMLoadFloat3(&position),     // The position of the "camera"
		dir,
		DirectX::XMLoadFloat3(&upDirection));     // "Up" direction in 3D space (prevents roll)
	DirectX::XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(view));
	DirectX::XMStoreFloat3(&direction, XMVector4Normalize(dir));
	DirectX::XMStoreFloat3(&newUp, XMVector4Normalize(up));
	
}

//-------------------------------------------------------------------
// Update View Matrix for 3rd person camera
//-------------------------------------------------------------------
void Camera::UpdateViewMatrix3P(DirectX::XMFLOAT3 target, DirectX::XMFLOAT3 rot, DirectX::XMVECTOR targetfDir)
{
	XMVECTOR pos = XMVectorSet(target.x, target.y + 5, target.z - 10.0f, 0);
	XMVECTOR dir = XMVectorSet(target.x, target.y, target.z, 0);
	XMVECTOR up = XMVector3Rotate(XMVectorSet(0, 1, 0, 0), XMLoadFloat3(&rot));
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&position),     // The position of the "camera"
		dir,
		DirectX::XMLoadFloat3(&upDirection));     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat3(&position, pos);
	DirectX::XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(view));
	DirectX::XMStoreFloat3(&direction, XMVector4Normalize(targetfDir));
	DirectX::XMStoreFloat3(&newUp, XMVector4Normalize(up));

}

//-------------------------------------------------------------------
// Return View Matrix
//-------------------------------------------------------------------
DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

//-------------------------------------------------------------------
// Update projection matrix
//-------------------------------------------------------------------
void Camera::UpdateProjectionMatrix(int width, int height)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL
}

//-------------------------------------------------------------------
// Return speed of camera
//-------------------------------------------------------------------
float Camera::GetSpeed() const
{
	return speed;
}

//-------------------------------------------------------------------
// Set speed of camera
//-------------------------------------------------------------------
void Camera::SetSpeed(float _speed)
{
	speed = _speed;
}

//-------------------------------------------------------------------
// Return type of camera
//-------------------------------------------------------------------
CameraType Camera::GetType() const
{
	return type;
}

//-------------------------------------------------------------------
// Set type of camera
//-------------------------------------------------------------------
void Camera::SetType(CameraType _type)
{
	type = _type;
}