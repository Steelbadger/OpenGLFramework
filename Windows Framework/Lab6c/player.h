#pragma once
#include "gameobject.h"
#include "collider.h"
#include "cameramodule.h"
#include "controller.h"

class Player : public GameObject
{
public:
	Player(void);
	~Player(void);

	void Update();


private:
	SphereCollider collider;
	Controller controller;
	CameraModule camera;


};

