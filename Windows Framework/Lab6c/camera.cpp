#include "camera.h"
#include "my4x4matrix.h"

Camera::Camera(void):
	Position(0,0,6,1),
	LookAt(0,0,0,1),
	GlobalUp(0,1,0,1)
{
	SetLocalVectors();
}

Camera::Camera(float xPos, float yPos, float zPos, float xLookAt, float yLookAt, float zLookAt):
	Position(xPos,yPos,zPos,1),
	LookAt(xLookAt, yLookAt, zLookAt, 1),
	GlobalUp(0,1,0,1)
{
	SetLocalVectors();
}

Camera::~Camera(void)
{
}

void Camera::Update()
{
	gluLookAt((GLdouble)Position.x, (GLdouble)Position.y, (GLdouble)Position.z,
				(GLdouble)LookAt.x, (GLdouble)LookAt.y, (GLdouble)LookAt.z,
				(GLdouble)upVector.x, (GLdouble)upVector.y, (GLdouble)upVector.z);

}


void Camera::RotateYaw(float Rotation)
{
	Matrix4x4 RotationMatrix;
	RotationMatrix.RotationAroundVectorThroughZero(upVector,Rotation);
	LookAt = LookAt - Position;
	LookAt = RotationMatrix * LookAt;
	LookAt = LookAt + Position;
	forwardVector = RotationMatrix * forwardVector;
	rightVector = RotationMatrix * rightVector;
}
void Camera::RotatePitch(float Rotation)
{
	Matrix4x4 RotationMatrix;
	RotationMatrix.RotationAroundVectorThroughZero(rightVector,Rotation);
	LookAt = LookAt - Position;
	LookAt = RotationMatrix * LookAt;
	LookAt = LookAt + Position;
	SetLocalVectors();
}
void Camera::RotateRoll(float Rotation)
{
	Matrix4x4 RotationMatrix;
	RotationMatrix.RotationAroundVectorThroughZero(forwardVector,Rotation);
	upVector = RotationMatrix * upVector;
	rightVector = RotationMatrix * rightVector;
}

void Camera::MoveForward(float Distance)
{
	Vector4 forwardTranslator = forwardVector.Normalise();
	forwardTranslator *= Distance;
	Matrix4x4 TranslationMatrix;
	TranslationMatrix.Translation(forwardTranslator.x, forwardTranslator.y, forwardTranslator.z);

	Position = TranslationMatrix * Position;
	LookAt = TranslationMatrix * LookAt;
}

void Camera::Strafe(float Distance)
{
	Vector4 strafeTranslator = rightVector.Normalise();
	strafeTranslator *= Distance;
	Matrix4x4 TranslationMatrix;
	TranslationMatrix.Translation(strafeTranslator.x, strafeTranslator.y, strafeTranslator.z);

	Position = TranslationMatrix * Position;
	LookAt = TranslationMatrix * LookAt;
}

void Camera::SetVectors(float xPos, float yPos, float zPos, float xLookAt, float yLookAt, float zLookAt)
{
	Position.x = xPos;
	Position.y = yPos;
	Position.z = zPos;
	Position.w = 1;
	LookAt.x = xLookAt;
	LookAt.y = yLookAt;
	LookAt.z = zLookAt;
	LookAt.w = 1;
	
	SetLocalVectors();
}

void Camera::SetGlobalUp(float x, float y, float z)
{
	GlobalUp.x = x;
	GlobalUp.y = y;
	GlobalUp.z = z;
	GlobalUp.w = 1;
	SetLocalVectors();

}

void Camera::SetLocalVectors()
{
	forwardVector = Position-LookAt;
	rightVector = forwardVector.Cross(GlobalUp);
	upVector = rightVector.Cross(forwardVector);
	upVector.NormaliseSelf();
}

