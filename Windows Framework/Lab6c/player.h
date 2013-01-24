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
	Controller controller;


private:
	SphereCollider collider;
	CameraModule camera;
	CameraController camController;
};

