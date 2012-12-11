#pragma once
#include "myvector4.h"
#include "my4x4matrix.h"
class GameObject
{
public:
	GameObject(void);
	~GameObject(void);
	void SetLocation(float x, float y, float z);
	void SetLocation(Vector3 Position);
	void SetLocation(Vector4 Position);
	void SetRotate(float xRot, float yRot, float zRot);

	void MoveDeltaX(float dx){position.x += dx;}
	void MoveDeltaY(float dy){position.y += dy;}
	void MoveDeltaZ(float dz){position.z += dz;}

	void RotateDeltaX(float dx);
	void RotateDeltaY(float dy);
	void RotateDeltaZ(float dz);

	Vector4 GetPosition(){return position;}
	Matrix4x4 GetRotation(){return rotation;}

	Vector4 position;
	Matrix4x4 rotation;
};

