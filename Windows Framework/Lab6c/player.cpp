#include "player.h"


Player::Player(void):
	collider(10.0f, GetPosition()),
		controller(this),
		camera(this),
		camController(&camera),
		flying(true)
{
	controller.SetKeyMap('W', 'W');
	controller.SetKeyMap('A', 'A');
	controller.SetKeyMap('S', 'S');
	controller.SetKeyMap('D', 'D');
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
		} else {
			controller.SetSensitivity(0.5f);
		}
	}
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
	if (!flying) {
		if (GetPosition().y < 2.0f+ground.GetHeight(GetPosition().x, GetPosition().z) || GetPosition().y > 2.0f+ground.GetHeight(GetPosition().x, GetPosition().z)) {
			SetLocation(GetPosition().x, 2.0f+ground.GetHeight(GetPosition().x, GetPosition().z), GetPosition().z);
		}
	}
}