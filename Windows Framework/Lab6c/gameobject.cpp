#include "gameobject.h"


GameObject::GameObject(void)
{
}


GameObject::~GameObject(void)
{
}

void GameObject::SetLocation(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	position.w = 1.0f;
}

void GameObject::SetLocation(Vector3 pos)
{
	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;
	position.w = 1.0f;
}

void GameObject::SetLocation(Vector4 pos)
{
	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;
	position.w = 1.0f;
}

void GameObject::SetRotate(float xRot, float yRot, float zRot)
{
	rotation = Matrix4x4::IDENTITY;
	Matrix4x4 temp;
	temp.RotationX(xRot);
	rotation = rotation * temp;
	temp.RotationY(yRot);
	rotation = rotation * temp;
	temp.RotationZ(zRot);
	rotation = rotation * temp;
}

void GameObject::RotateDeltaX(float xRot)
{
	Matrix4x4 temp;
	temp.RotationX(xRot);
	rotation = rotation * temp;
}

void GameObject::RotateDeltaY(float yRot)
{
	Matrix4x4 temp;
	temp.RotationY(yRot);
	rotation = rotation * temp;
}
void GameObject::RotateDeltaZ(float zRot)
{
	Matrix4x4 temp;
	temp.RotationZ(zRot);
	rotation = rotation * temp;
}