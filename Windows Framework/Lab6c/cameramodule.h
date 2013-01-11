#pragma once
#include "myvector4.h"
#include "gameobject.h"
#include "mywindow.h"
#include <windows.h>
#include <mmsystem.h>
#include <gl/gl.h>
#include <gl/glu.h>

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
	void LockCursorToCentre();

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

	Vector4 LookAt;
};

