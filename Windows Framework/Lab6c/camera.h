#pragma once

#include "myvector4.h"
#include <windows.h>
#include <mmsystem.h>
#include <gl/gl.h>
#include <gl/glu.h>



class Camera
{
public:
	Camera(void);
	Camera(float xPos, float yPos, float zPos, float xLookAt, float yLookAt, float zLookAt);
	~Camera(void);

	void SetVectors(float xPos, float yPos, float zPos, float xLookAt, float yLookAt, float zLookAt);
	void SetGlobalUp(float x, float y, float z);

	void Update();

	void RotateYaw(float Rotation);
	void RotatePitch(float Rotation);
	void RotateRoll(float Rotation);

	void MoveForward(float Distance);
	void Strafe(float Distance);

	float GetCameraX(){return Position.x;}
	float GetCameraY(){return Position.y;}
	float GetCameraZ(){return Position.z;}
	
private:
	void SetLocalVectors();

	Vector4 forwardVector;
	Vector4 upVector;
	Vector4 rightVector;
	Vector4 Position;
	Vector4 LookAt;
	Vector4 GlobalUp;
};

