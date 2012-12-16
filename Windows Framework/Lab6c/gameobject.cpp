#include "gameobject.h"


const Vector4 GameObject::GLOBALX(1.0f, 0.0f, 0.0f, 1.0f);
const Vector4 GameObject::GLOBALY(0.0f, 1.0f, 0.0f, 1.0f);
const Vector4 GameObject::GLOBALZ(0.0f, 0.0f, 1.0f, 1.0f);


GameObject::GameObject(void):
	position(0.0f, 0.0f, 0.0f, 1.0f),
		rotation(Matrix4x4::IDENTITY)
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
	Vector4 localY;
	Vector4 localX;
	Vector4 localZ;
	localZ = rotation * GLOBALZ;

	localX = localZ.Cross(GLOBALY);
	localY = localX.Cross(localZ);

	Vector4 rotationCorrectionVector(localY.y, -localY.x, 0.0f, 1.0f);
	float rotationCorrectionAngle = acos(localY.z);

	Matrix4x4 rotationCorrectionMatrix;
	rotationCorrectionMatrix.RotationAroundVectorThroughZero(rotationCorrectionVector, rotationCorrectionAngle);
	rotation = rotation * rotationCorrectionMatrix;
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
	Vector4 xTranslator;
	xTranslator = rotation * GLOBALX;
	xTranslator.NormaliseSelf();
	xTranslator *= dx;

	Matrix4x4 translationMatrix;
	translationMatrix.Translation(xTranslator.x, xTranslator.y, xTranslator.z);

	position =  translationMatrix * position;
}
void GameObject::MoveLocalDeltaY(float dy)
{
	Vector4 yTranslator;
	yTranslator = rotation * GLOBALY;
	yTranslator.NormaliseSelf();
	yTranslator *= dy;

	Matrix4x4 translationMatrix;
	translationMatrix.Translation(yTranslator.x, yTranslator.y, yTranslator.z);

	position =  translationMatrix * position;
}
void GameObject::MoveLocalDeltaZ(float dz)
{
	Vector4 zTranslator;
	zTranslator = rotation * GLOBALZ;
	zTranslator.NormaliseSelf();
	zTranslator *= dz;

	Matrix4x4 translationMatrix;
	translationMatrix.Translation(zTranslator.x, zTranslator.y, zTranslator.z);

	position =  translationMatrix * position;
}

void GameObject::RotateDeltaX(float dx)
{
	Matrix4x4 temp;
	temp.RotationX(dx);
	rotation = rotation * temp;
}
void GameObject::RotateDeltaY(float dy)
{
	Matrix4x4 temp;
	temp.RotationX(dy);
	rotation = rotation * temp;
}
void GameObject::RotateDeltaZ(float dz)
{
	Matrix4x4 temp;
	temp.RotationX(dz);
	rotation = rotation * temp;
}

void GameObject::RotateLocalDeltaX(float dx)
{
	Vector4 xVector;
	xVector = rotation * GLOBALX;
	Matrix4x4 deltaRotation;
	deltaRotation.RotationAroundVectorThroughZero(xVector,dx);
	rotation = rotation * deltaRotation;
}
void GameObject::RotateLocalDeltaY(float dy)
{
	Vector4 yVector;
	yVector = rotation * GLOBALY;
	Matrix4x4 deltaRotation;
	deltaRotation.RotationAroundVectorThroughZero(yVector,dy);
	rotation = rotation * deltaRotation;
}
void GameObject::RotateLocalDeltaZ(float dz)
{
	Vector4 zVector;
	zVector = rotation * GLOBALZ;
	Matrix4x4 deltaRotation;
	deltaRotation.RotationAroundVectorThroughZero(zVector,dz);
	rotation = rotation * deltaRotation;
}

Vector4& GameObject::GetPosition()
{
	return position;
}

Matrix4x4& GameObject::GetRotation()
{
	return rotation;
}