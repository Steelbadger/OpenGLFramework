#pragma once
#include "gameobject.h"
#include "collider.h"
#include "cameramodule.h"
#include "controller.h"
#include "cameracontroller.h"
#include "mesh.h"

class Player : public GameObject
{
public:
	Player(void);
	~Player(void);


	void InputUpdate();
	void Update();
	void SetCameraTargetWindow(WindowWizard* window);
	Controller controller;

	void CheckGroundCollision(Mesh &ground);


private:
	bool flying;

	SphereCollider collider;
	CameraModule camera;
	CameraController camController;
};

