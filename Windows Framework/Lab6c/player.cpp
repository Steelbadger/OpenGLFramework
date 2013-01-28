#include "player.h"


Player::Player(void):
	collider(2.0f, GetPosition()),
		controller(this),
		camera(this),
		camController(&camera),
		flying(true),
		rigidbody(this),
		skybox("skybox.obj", "skybox.tga", this)
{
}


Player::~Player(void)
{
}

void Player::InputUpdate()
{
	controller.CheckInputAndAct();
	camController.CheckInputAndUpdate();
	if (input.ReportKeyPress('T')) {
		flying = !flying;
		if (flying == false){
			controller.SetSensitivity(0.3f);
			rigidbody.SetActive();
		} else {
			controller.SetSensitivity(2.5f);
			rigidbody.SetInactive();
		}
	}
	rigidbody.Update();
}


void Player::Update()
{


	camera.Update();
}

void Player::SetCameraTargetWindow(WindowWizard* window)
{
	camera.SetTargetWindow(window);
}

void Player::CheckGroundCollision(Terrain &ground)
{

	if (rigidbody.CheckGroundCollision(ground)) {
		controller.SetJumping(false);
	}
}

Collider* Player::GetCollider()
{
	return &collider;
}

CameraModule* Player::GetCamera()
{
	return &camera;
}

Rigidbody* Player::GetRigidbody()
{
	return &rigidbody;
}

Controller* Player::GetController()
{
	return &controller;
}

Terrain* Player::GetTerrain()
{
	return NULL;
}

void Player::DrawSkyBox()
{
	skybox.Draw();
}