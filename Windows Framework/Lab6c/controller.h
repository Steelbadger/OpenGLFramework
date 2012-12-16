#pragma once
#include "gameobject.h"
class Controller
{
public:
	Controller(GameObject* parent);
	~Controller(void);

	void CheckInputAndAct();
	void SetKeyMap(char KeyToMap, char KeyToMapTo);

private:
	GameObject* parent;
	char keyMap[256];
};

