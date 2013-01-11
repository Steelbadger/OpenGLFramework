#pragma once
#include "myvector4.h"
#include "my4x4matrix.h"

class GameObject
{
public:
	GameObject(void);
	~GameObject(void);
	void SetLocation(float x, float y, float z);
	void SetRotate(float xRot, float yRot, float zRot);
	void OrientateAxesToGlobalUp();

	void MoveDeltaX(float dx);
	void MoveDeltaY(float dy);
	void MoveDeltaZ(float dz);

	void MoveLocalDeltaX(float dx);
	void MoveLocalDeltaY(float dy);
	void MoveLocalDeltaZ(float dz);

	void RotateDeltaX(float dx);
	void RotateDeltaY(float dy);
	void RotateDeltaZ(float dz);

	void RotateLocalDeltaX(float dx);
	void RotateLocalDeltaY(float dy);
	void RotateLocalDeltaZ(float dz);

	Vector4& GetPosition();
	Vector4 GetLocalX();
	Vector4 GetLocalY();
	Vector4 GetLocalZ();

	Matrix4x4& GetRotation();

private:
	Vector4 position;
	Matrix4x4 rotation;
	Vector4 localX;
	Vector4 localY;
	Vector4 localZ;

	static const Vector4 GLOBALX;
	static const Vector4 GLOBALY;
	static const Vector4 GLOBALZ;
};



