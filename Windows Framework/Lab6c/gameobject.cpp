#include "gameobject.h"


const Vector4 GameObject::GLOBALX(1.0f, 0.0f, 0.0f, 1.0f);
const Vector4 GameObject::GLOBALY(0.0f, 1.0f, 0.0f, 1.0f);
const Vector4 GameObject::GLOBALZ(0.0f, 0.0f, 1.0f, 1.0f);


GameObject::GameObject(void):
	position(0.0f, 0.0f, 0.0f, 1.0f),
		rotation(Matrix4x4::IDENTITY),
		localX(1.0f, 0.0f, 0.0f, 1.0f),
		localY(0.0f, 1.0f, 0.0f, 1.0f),
		localZ(0.0f, 0.0f, 1.0f, 1.0f)
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
	Matrix4x4 temp;
	rotation.RotationX(xRotNew);
	temp.RotationY(yRotNew);
	rotation = rotation * temp;
	temp.RotationZ(zRotNew);
	rotation = rotation * temp;

}

void GameObject::OrientateAxesToGlobalUp()
{
	localX = localZ.Cross(GLOBALY);
	localY = localX.Cross(localZ);
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
	Vector4 translator = localX.Normalise();
	translator *= dx;
	Matrix4x4 TranslationMatrix;
	TranslationMatrix.Translation(translator.x, translator.y, translator.z);

	position = TranslationMatrix * position;
}
void GameObject::MoveLocalDeltaY(float dy)
{
	Vector4 translator = localY.Normalise();
	translator *= dy;
	Matrix4x4 TranslationMatrix;
	TranslationMatrix.Translation(translator.x, translator.y, translator.z);

	position = TranslationMatrix * position;
}
void GameObject::MoveLocalDeltaZ(float dz)
{
	Vector4 translator = localZ.Normalise();
	translator *= dz;
	Matrix4x4 TranslationMatrix;
	TranslationMatrix.Translation(translator.x, translator.y, translator.z);

	position = TranslationMatrix * position;
}

void GameObject::RotateDeltaX(float dx)
{
	Matrix4x4 RotationMatrix;
	RotationMatrix.RotationAroundVectorThroughZero(GLOBALX,dx);
	localZ = RotationMatrix * localZ;
	localY = RotationMatrix * localY;
	localX = RotationMatrix * localX;
}
void GameObject::RotateDeltaY(float dy)
{
	Matrix4x4 RotationMatrix;
	RotationMatrix.RotationAroundVectorThroughZero(GLOBALY,dy);
	localZ = RotationMatrix * localZ;
	localY = RotationMatrix * localY;
	localX = RotationMatrix * localX;
}
void GameObject::RotateDeltaZ(float dz)
{
	Matrix4x4 RotationMatrix;
	RotationMatrix.RotationAroundVectorThroughZero(GLOBALZ,dz);
	localZ = RotationMatrix * localZ;
	localY = RotationMatrix * localY;
	localX = RotationMatrix * localX;
}

void GameObject::RotateLocalDeltaX(float dx)
{
	Matrix4x4 RotationMatrix;
	RotationMatrix.RotationAroundVectorThroughZero(localX,dx);
	localZ = RotationMatrix * localZ;
	localY = RotationMatrix * localY;
}
void GameObject::RotateLocalDeltaY(float dy)
{
	Matrix4x4 RotationMatrix;
	RotationMatrix.RotationAroundVectorThroughZero(localY,dy);
	localZ = RotationMatrix * localZ;
	localX = RotationMatrix * localX;
}
void GameObject::RotateLocalDeltaZ(float dz)
{
	Matrix4x4 RotationMatrix;
	RotationMatrix.RotationAroundVectorThroughZero(localZ,dz);
	localY = RotationMatrix * localY;
	localX = RotationMatrix * localX;
}

Vector4& GameObject::GetPosition()
{
	return position;
}

Vector4 GameObject::GetLocalX()
{
	return localX;
}

Vector4 GameObject::GetLocalY()
{
	return localY;
}

Vector4 GameObject::GetLocalZ()
{
	return localZ;
}

Matrix4x4& GameObject::GetRotation()
{
	return rotation;
}