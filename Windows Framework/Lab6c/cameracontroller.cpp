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
	if (input.WindowResized()) {
		camera->SetWindowSize();
	}
}
