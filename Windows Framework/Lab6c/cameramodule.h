#pragma once
#include "myvector4.h"
#include "gameobject.h"
#include <windows.h>
#include <mmsystem.h>
#include <gl/gl.h>
#include <gl/glu.h>

class CameraModule
{
public:
	CameraModule(GameObject* parent);
	~CameraModule(void);

	void SetWindowSize(int width, int height);
	void SetClipPlanes(float near, float far);
	void SetFieldOfView(float fov);

	void Update();

	void Zoom(int zoom);

private:
	GameObject* parent;

	int windowWidth;
	int windowHeight;
	float fieldOfView;
	float nearClipPlane;
	float farClipPlane;

	Vector4 LookAt;
};

