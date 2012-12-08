#include "gameobject.h"


GameObject::GameObject(void)
{
	x = 0;
	y = 0;
	z = 0;
}


GameObject::~GameObject(void)
{
}

void GameObject::SetLocation(float xNew, float yNew, float zNew)
{
	x = xNew;
	y = yNew;
	z = zNew;
}
void GameObject::SetRotate(float xRotNew, float yRotNew, float zRotNew)
{
	xRot = xRotNew;
	yRot = yRotNew;
	zRot = zRotNew;
}
void GameObject::MoveDeltaX(float dx)
{
	x += dx;
}
void GameObject::MoveDeltaY(float dy)
{
	y += dy;
}
void GameObject::MoveDeltaZ(float dz)
{
	z += dz;
}

void GameObject::RotateDeltaX(float dx)
{
	xRot += dx;
}
void GameObject::RotateDeltaY(float dy)
{
	yRot += dy;
}
void GameObject::RotateDeltaZ(float dz)
{
	zRot += dz;
}

float GameObject::GetX()
{
	return x;
}
float GameObject::GetY()
{
	return y;
}
float GameObject::GetZ()
{
	return z;
}

float GameObject::GetRotationX()
{
	return xRot;
}
float GameObject::GetRotationY()
{
	return yRot;
}
float GameObject::GetRotationZ()
{
	return zRot;
}