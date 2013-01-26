#include "player.h"


Player::Player(void):
	collider(2.0f, GetPosition()),
		controller(this),
		camera(this),
		camController(&camera),
		flying(true),
		rigidbody(this)
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
			controller.SetSensitivity(0.1f);
			rigidbody.SetActive();
		} else {
			controller.SetSensitivity(0.5f);
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

void Player::CheckGroundCollision(Mesh &ground)
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

Mesh* Player::GetMesh()
{
	return NULL;
}