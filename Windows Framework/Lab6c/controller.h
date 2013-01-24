#pragma once
#include "gameobject.h"
class Controller
{
public:
	Controller(GameObject* parent);
	~Controller(void);

	void CheckInputAndAct();
	void SetKeyMap(char KeyToMap, char KeyToMapTo);
	void SetSensitivity(float s){sensitivity = s;}

private:
	GameObject* parent;
	char keyMap[256];
	float sensitivity;
};

