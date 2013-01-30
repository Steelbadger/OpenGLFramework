#include "player.h"
#include "noisegenerator.h"


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
			controller.SetSensitivity(8.0f);
			rigidbody.SetActive();
		} else {
			controller.SetSensitivity(50.0f);
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

void Player::CheckGroundCollision(NoiseObject n)
{

	if (rigidbody.CheckGroundCollision(n)) {
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
	return &skybox;
}

void Player::DrawSkyBox()
{
//	skybox.Draw();
}