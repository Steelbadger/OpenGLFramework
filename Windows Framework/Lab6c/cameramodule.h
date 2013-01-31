#pragma once
//  Author - Ross Davies
//  This class turns the GameObject derived class to which
//  it is attached into an OpenGL camera.  There are no functions
//  for moving the camera, any movement on the camera is handled 
//  by the parent GameObject.

#include "mywindow.h"
#include <windows.h>
#include "myvector4.h"
#include "my4x4matrix.h"

class GameObject;

class CameraModule
{
public:
	CameraModule(GameObject* parent);
	~CameraModule(void);

	void SetTargetWindow(HWND hWnd);
	void SetTargetWindow(WindowWizard* window);
	void SetWindowSize(int width, int height);
	void SetWindowSize();
	void SetClipPlanes(float near, float far);
	void SetFieldOfView(float fov);
	float GetFieldOfView(){return fieldOfView;}
	void LockCursorToCentre();
	int GetWindowWidth(){return windowWidth;}
	int GetWindowHeight(){return windowHeight;}
	float GetNearClipPlane(){return nearClipPlane;}
	float GetFarClipPlane(){return farClipPlane;}
	void CalculateModelMatrix();

	GameObject* GetParent(){return parent;}

	void Update();

	void Zoom(int zoom);

private:
	GameObject* parent;
	WindowWizard* targetWindow;

	int windowWidth;
	int windowHeight;
	float fieldOfView;
	float nearClipPlane;
	float farClipPlane;

	Matrix4x4 model;
	Matrix4x4 projection;

	Vector4 LookAt;
};

