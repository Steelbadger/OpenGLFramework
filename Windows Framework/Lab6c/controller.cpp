#include "controller.h"
#include "input.h"


Controller::Controller(GameObject* p):
	parent(p),
	sensitivity(0.5f)
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
		parent->MoveLocalDeltaZ(sensitivity);
	if(input.ReportKeyState(keyMap['S']))
		parent->MoveLocalDeltaZ(-sensitivity);

	if (input.GetMouseR()) {
		parent->RotateLocalDeltaY(-(float)input.GetMouseDX()/1000.0);
		parent->RotateLocalDeltaX(-(float)input.GetMouseDY()/1000.0);
		if (input.ReportKeyState(keyMap['D']))
			parent->MoveLocalDeltaX(sensitivity);
		if (input.ReportKeyState(keyMap['A']))
			parent->MoveLocalDeltaX(-sensitivity);
		parent->OrientateAxesToGlobalUp();
	} else {
		if (input.ReportKeyState(keyMap['D']))
			parent->RotateLocalDeltaY(-sensitivity/5);
		if (input.ReportKeyState(keyMap['A']))
			parent->RotateLocalDeltaY(sensitivity/5);
		parent->OrientateAxesToGlobalUp();
	}
}

void Controller::SetKeyMap(char keyToMap, char keyToMapTo)
{
	keyMap[keyToMap] = keyToMapTo;
}
