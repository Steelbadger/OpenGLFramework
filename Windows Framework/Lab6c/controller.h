#pragma once

class GameObject;

class Controller
{
public:
	Controller(GameObject* parent);
	~Controller(void);

	void CheckInputAndAct();
	void SetSensitivity(float s){sensitivity = s;}
	void SetJumping(bool j){jumping = j;}

private:
	GameObject* parent;
	float sensitivity;

	bool jumping;
};

