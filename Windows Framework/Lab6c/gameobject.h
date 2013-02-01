#pragma once
#include "myvector4.h"
#include "collider.h"
#include "cameramodule.h"
#include "rigidbodymodule.h"
#include "Terrain.h"
#include "controller.h"
#include "mesh.h"
#include "quaternion.h"

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

	Quaternion GetRotation();

	virtual Collider* GetCollider();
	virtual CameraModule* GetCamera();
	virtual Rigidbody* GetRigidbody();
	virtual Mesh* GetMesh();
	virtual Controller* GetController();

	static const Vector4 GLOBALX;
	static const Vector4 GLOBALY;
	static const Vector4 GLOBALZ;
private:
	Vector4 position;
	Quaternion rotation;
	Vector4 localX;
	Vector4 localY;
	Vector4 localZ;
};



