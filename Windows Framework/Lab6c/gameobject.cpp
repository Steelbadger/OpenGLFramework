#include "gameobject.h"
#include "myvector4.h"
#include "collider.h"
#include "cameramodule.h"
#include "rigidbodymodule.h"
#include "mesh.h"
#include "controller.h"
#include "quaternion.h"

const Vector4 GameObject::GLOBALX(1.0f, 0.0f, 0.0f, 1.0f);
const Vector4 GameObject::GLOBALY(0.0f, 1.0f, 0.0f, 1.0f);
const Vector4 GameObject::GLOBALZ(0.0f, 0.0f, 1.0f, 1.0f);


GameObject::GameObject(void):
	position(0.0f, 0.0f, 0.0f, 1.0f),
		rotation(),
		localX(GLOBALX),
		localY(GLOBALY),
		localZ(GLOBALZ),
		scale(1.0f, 1.0f, 1.0f),
		parent(NULL)
{
}


GameObject::~GameObject(void)
{
}

void GameObject::SetLocation(float xNew, float yNew, float zNew)
{
	position.x = xNew;
	position.y = yNew;
	position.z = zNew;
	position.w = 1.0f;
}
void GameObject::SetRotate(float xRotNew, float yRotNew, float zRotNew)
{
	Quaternion xRotation;
	Quaternion yRotation;
	Quaternion zRotation;

	if (xRotNew == 0.0f) {
		xRotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
	} else {
		xRotation.Rotation(xRotNew, 1.0f, 0.0f, 0.0f);
	}

	if (yRotNew == 0.0f) {
		yRotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
	} else {
		yRotation.Rotation(yRotNew, 0.0f, 1.0f, 0.0f);
	}

	if (zRotNew == 0.0f) {
		zRotation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
	} else {
		zRotation.Rotation(zRotNew, 0.0f, 0.0f, 1.0f);
	}

	rotation = xRotation * yRotation * zRotation;
	rotation.NormalizeSelf();
}

void GameObject::SetRotate(Quaternion q) 
{
	rotation = q;
	rotation.NormalizeSelf();
}

void GameObject::MoveDeltaX(float dx)
{
	position.x += dx;
}
void GameObject::MoveDeltaY(float dy)
{
	position.y += dy;
}
void GameObject::MoveDeltaZ(float dz)
{
	position.z += dz;
}

void GameObject::MoveLocalDeltaX(float dx)
{
	localX = rotation * Quaternion(GLOBALX) * rotation.Inverse();
	Vector4 translator = localX.Normalise();
	translator *= dx;
	Matrix4x4 TranslationMatrix;
	TranslationMatrix.Translation(translator.x, translator.y, translator.z);

	position = TranslationMatrix * position;
}
void GameObject::MoveLocalDeltaY(float dy)
{
	localY = rotation * Quaternion(GLOBALY) * rotation.Inverse();
	Vector4 translator = localY.Normalise();
	translator *= dy;
	Matrix4x4 TranslationMatrix;
	TranslationMatrix.Translation(translator.x, translator.y, translator.z);

	position = TranslationMatrix * position;
}
void GameObject::MoveLocalDeltaZ(float dz)
{
	localZ = rotation * Quaternion(GLOBALZ) * rotation.Inverse();
	Vector4 translator = localZ.Normalise();
	translator *= dz;
	Matrix4x4 TranslationMatrix;
	TranslationMatrix.Translation(translator.x, translator.y, translator.z);

	position = TranslationMatrix * position;
}

void GameObject::RotateDeltaX(float dx)
{
	Quaternion rot;
	rot.Rotation(dx, GLOBALX);
	rotation = rot * rotation;
	rotation.NormalizeSelf();

	localX = rotation * Quaternion(GLOBALX) * rotation.Inverse();
	localY = rotation * Quaternion(GLOBALY) * rotation.Inverse();
	localZ = rotation * Quaternion(GLOBALZ) * rotation.Inverse();
}
void GameObject::RotateDeltaY(float dy)
{
	Quaternion rot;
	rot.Rotation(dy, GLOBALY);
	rotation = rot * rotation;
	rotation.NormalizeSelf();

	localX = rotation * Quaternion(GLOBALX) * rotation.Inverse();
	localY = rotation * Quaternion(GLOBALY) * rotation.Inverse();
	localZ = rotation * Quaternion(GLOBALZ) * rotation.Inverse();

	localX.NormaliseSelf();
	localY.NormaliseSelf();
	localZ.NormaliseSelf();

}
void GameObject::RotateDeltaZ(float dz)
{
	Quaternion rot;
	rot.Rotation(dz, GLOBALZ);
	rotation = rot * rotation;
	rotation.NormalizeSelf();

	localX = rotation * Quaternion(GLOBALX) * rotation.Inverse();
	localY = rotation * Quaternion(GLOBALY) * rotation.Inverse();
	localZ = rotation * Quaternion(GLOBALZ) * rotation.Inverse();
}

void GameObject::RotateLocalDeltaX(float dx)
{
	Vector4 rotationAxis = rotation * Quaternion(GLOBALX) * rotation.Inverse();

	Quaternion rot;
	rot.Rotation(dx, rotationAxis);
	rotation = rot * rotation;
	rotation.NormalizeSelf();

	localX = rotation * Quaternion(GLOBALX) * rotation.Inverse();
	localY = rotation * Quaternion(GLOBALY) * rotation.Inverse();
	localZ = rotation * Quaternion(GLOBALZ) * rotation.Inverse();
}
void GameObject::RotateLocalDeltaY(float dy)
{
	Vector4 rotationAxis = rotation * Quaternion(GLOBALY) * rotation.Inverse();

	Quaternion rot;
	rot.Rotation(dy, rotationAxis);
	rotation = rot * rotation;
	rotation.NormalizeSelf();

	localX = rotation * Quaternion(GLOBALX) * rotation.Inverse();
	localY = rotation * Quaternion(GLOBALY) * rotation.Inverse();
	localZ = rotation * Quaternion(GLOBALZ) * rotation.Inverse();
}
void GameObject::RotateLocalDeltaZ(float dz)
{
	Vector4 rotationAxis = rotation * Quaternion(GLOBALZ) * rotation.Inverse();

	Quaternion rot;
	rot.Rotation(dz, rotationAxis);
	rotation = rot * rotation;
	rotation.NormalizeSelf();

	localX = rotation * Quaternion(GLOBALX) * rotation.Inverse();
	localY = rotation * Quaternion(GLOBALY) * rotation.Inverse();
	localZ = rotation * Quaternion(GLOBALZ) * rotation.Inverse();
}

Vector4 GameObject::GetPosition()
{
	if (parent == NULL) {
		return position;
	} else {
		return position + parent->GetPosition();
	}
}

Vector4 GameObject::GetLocalX()
{
	if (parent == NULL) {
		return localX;
	} else {
		return parent->GetRotation() * Quaternion(localX) * parent->GetRotation().Inverse();
	}
}

Vector4 GameObject::GetLocalY()
{
	if (parent == NULL) {
		return localY;
	} else {
		return parent->GetRotation() * Quaternion(localY) * parent->GetRotation().Inverse();
	}
}

Vector4 GameObject::GetLocalZ()
{
	if (parent == NULL) {
		return localZ;
	} else {
		return parent->GetRotation() * Quaternion(localZ) * parent->GetRotation().Inverse();
	}
}

Quaternion GameObject::GetRotation()
{
	if (parent == NULL) {
		return rotation;
	} else {
		return parent->GetRotation() * rotation;
	}
}

Collider* GameObject::GetCollider()
{
	return NULL;
}

CameraModule* GameObject::GetCamera()
{
	return NULL;
}

Rigidbody* GameObject::GetRigidbody()
{
	return NULL;
}

Mesh* GameObject::GetMesh()
{
	return NULL;
}

Controller* GameObject::GetController()
{
	return NULL;
}

GameObject* GameObject::GetParent()
{
	return parent;
}

void GameObject::SetParent(GameObject &g)
{
	parent = &g;
}

void GameObject::UniformScale(float scaleFactor)
{
	scale = Vector4(scale) * scaleFactor;
}

void GameObject::SetScale(float scaleFactor)
{
	scale.x = scaleFactor;
	scale.y = scaleFactor;
	scale.z = scaleFactor;
}

void GameObject::NonUniformScale(float xScale, float yScale, float zScale)
{
	scale.x *= xScale;
	scale.y *= yScale;
	scale.z *= zScale;
}

