#include "player.h"


Player::Player(void):
	collider(10.0f, GetPosition()),
		controller(this),
		camera(this)
{
}


Player::~Player(void)
{
}


void Player::Update()
{
	controller.CheckInputAndAct();
	camera.Update();
}