#include "cameramodule.h"
#include "gameobject.h"


CameraModule::CameraModule(GameObject* parent):
	parent(parent),
	fieldOfView(45.0f),
	nearClipPlane(0.1f),
	farClipPlane(500.0f)
{
}


CameraModule::~CameraModule(void)
{
}

void CameraModule::Zoom(int zoom)
{
	fieldOfView -= zoom/120;
	if (fieldOfView > 45.0f) {
		fieldOfView = 45.0f;
	} else if (fieldOfView < 1.0f) {
		fieldOfView = 1.0f;
	}
}

void CameraModule::SetWindowSize(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
}

void CameraModule::SetWindowSize()
{
	if (targetWindow != NULL) {
		windowWidth = targetWindow->GetWidth();
		windowHeight = targetWindow->GetHeight();
	}
}

void CameraModule::SetClipPlanes(float n, float f)
{
	nearClipPlane = n;
	farClipPlane = f;
}

void CameraModule::SetFieldOfView(float fov)
{
	fieldOfView = fov;
}

void CameraModule::SetTargetWindow(HWND hWnd)
{
	targetWindow = WindowWizard::GetWindowReference(hWnd);
	SetWindowSize();
}

void CameraModule::SetTargetWindow(WindowWizard* window)
{
	targetWindow = window;
	SetWindowSize();
}

void CameraModule::LockCursorToCentre()
{
	if (targetWindow != NULL) {
		targetWindow->SetCursorToCentre();
	}
}

void CameraModule::CalculateModelMatrix()
{
	
}

Vector4 CameraModule::GetForward()
{
	return parent->GetLocalZ();
}

Vector4 CameraModule::GetUp()
{
	Vector4 up = (GetForward().Cross(GameObject::GLOBALY)).Cross(GetForward());

	return up;
}

Vector4 CameraModule::GetPosition()
{
	return parent->GetPosition();
}