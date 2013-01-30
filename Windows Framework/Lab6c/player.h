#pragma once
//#include "rigidbodymodule.h"
#include "gameobject.h"
#include "collider.h"
#include "cameramodule.h"
#include "controller.h"
#include "cameracontroller.h"
#include "Terrain.h"
#include "mesh.h"

class NoiseObject;

class Player : public GameObject
{
public:
	Player(void);
	~Player(void);


	void InputUpdate();
	void Update();
	void SetCameraTargetWindow(WindowWizard* window);
	Controller controller;

	void CheckGroundCollision(NoiseObject n);

	virtual Collider* GetCollider();
	virtual CameraModule* GetCamera();
	virtual Rigidbody* GetRigidbody();
	virtual Mesh* GetMesh();
	virtual Controller* GetController();

//	void InitSkyBox(){skybox.Initialize();}

	void DrawSkyBox();

private:
	bool flying;

	SphereCollider collider;
	CameraModule camera;
	CameraController camController;
	Rigidbody rigidbody;
	Mesh skybox;

};

