#include "cameracontroller.h"
#include "input.h"


CameraController::CameraController(CameraModule* cam):
	camera(cam)
{
}


CameraController::~CameraController(void)
{
}

void CameraController::CheckInputAndUpdate()
{
	if (input.CheckMouseWheel()) {
		camera->Zoom(input.GetMouseWheelDelta());
	}
	if (input.GetMouseR()) {
		camera->LockCursorToCentre();
	}

	if (input.ReportKeyState(VK_UP)) {
		camera->SetClipPlanes(camera->GetNearClipPlane(), camera->GetFarClipPlane()+10.0f);
	}
	if (input.ReportKeyState(VK_DOWN)) {
		camera->SetClipPlanes(camera->GetNearClipPlane(), camera->GetFarClipPlane()-10.0f);
	}
	if (input.WindowResized()) {
		camera->SetWindowSize();
	}
}
