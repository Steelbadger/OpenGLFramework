#include "player.h"


Player::Player(void):
	collider(10.0f, GetPosition()),
		controller(this),
		camera(this),
		camController(&camera)
{
	controller.SetKeyMap('W', 'W');
	controller.SetKeyMap('A', 'A');
	controller.SetKeyMap('S', 'S');
	controller.SetKeyMap('D', 'D');
}


Player::~Player(void)
{
}


void Player::Update()
{
	controller.CheckInputAndAct();
	camController.CheckInputAndUpdate();
	camera.Update();
}

void Player::SetCameraTargetWindow(WindowWizard* window)
{
	camera.SetTargetWindow(window);
}