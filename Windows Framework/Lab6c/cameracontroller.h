#pragma once
#include "cameramodule.h"

class CameraController
{
public:
	CameraController(CameraModule* camera);
	~CameraController(void);

	void CheckInputAndUpdate();

private:
	CameraModule* camera;
};

