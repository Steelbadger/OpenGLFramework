#pragma once
#include "gameobject.h"
#include "collider.h"
#include "cameramodule.h"
#include "controller.h"
#include "cameracontroller.h"

class Player : public GameObject
{
public:
	Player(void);
	~Player(void);

	void Update();
	void SetCameraTargetWindow(WindowWizard* window);

private:
	SphereCollider collider;
	CameraModule camera;
	Controller controller;
	CameraController camController;
};

