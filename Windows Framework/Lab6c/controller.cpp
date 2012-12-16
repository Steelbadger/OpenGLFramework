#include "controller.h"
#include "input.h"


Controller::Controller(GameObject* p):
	parent(p)
{
	for (int i = 0; i < 256; i++) {
		keyMap[i] = 0;
	}
}


Controller::~Controller(void)
{
}

void Controller::CheckInputAndAct()
{
	if (input.ReportKeyState(keyMap['W']))
		parent->MoveLocalDeltaZ(-0.04f);
	if(input.ReportKeyState(keyMap['S']))
		parent->MoveLocalDeltaZ(0.04f);

	if (!input.GetMouseR()) {
		if (input.ReportKeyState(keyMap['D']))
			parent->RotateLocalDeltaY(-0.01f);
		if (input.ReportKeyState(keyMap['A']))
			parent->RotateLocalDeltaY(0.01f);
	}
}

void Controller::SetKeyMap(char keyToMap, char keyToMapTo)
{
	keyMap[keyToMap] = keyToMapTo;
}
