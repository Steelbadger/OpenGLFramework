#include "cameramodule.h"


CameraModule::CameraModule(GameObject* parent):
	parent(parent),
	fieldOfView(45.0f),
	nearClipPlane(0.001f),
	farClipPlane(100.0f)
{
}


CameraModule::~CameraModule(void)
{
}

void CameraModule::Update()
{
	Vector4 Position = parent->GetPosition();
	Vector4 upVector = parent->GetLocalY();
	LookAt = Position + parent->GetLocalZ();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//calculate aspect ratio
	gluPerspective(fieldOfView, (GLfloat)windowWidth/(GLfloat)windowHeight, nearClipPlane, farClipPlane);
	glMatrixMode(GL_MODELVIEW);// Select The Modelview Matrix
	glLoadIdentity();
	gluLookAt((GLdouble)Position.x, (GLdouble)Position.y, (GLdouble)Position.z,
				(GLdouble)LookAt.x, (GLdouble)LookAt.y, (GLdouble)LookAt.z,
				(GLdouble)upVector.x, (GLdouble)upVector.y, (GLdouble)upVector.z);

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