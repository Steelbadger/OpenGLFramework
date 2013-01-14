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
		parent->MoveLocalDeltaZ(0.04f);
	if(input.ReportKeyState(keyMap['S']))
		parent->MoveLocalDeltaZ(-0.04f);

	if (input.GetMouseR()) {
		parent->RotateLocalDeltaY(-(float)input.GetMouseDX()/1000.0);
		parent->RotateLocalDeltaX(-(float)input.GetMouseDY()/1000.0);
		if (input.ReportKeyState(keyMap['D']))
			parent->MoveLocalDeltaX(0.03f);
		if (input.ReportKeyState(keyMap['A']))
			parent->MoveLocalDeltaX(-0.03f);
		parent->OrientateAxesToGlobalUp();
	} else {
		if (input.ReportKeyState(keyMap['D']))
			parent->RotateLocalDeltaY(-0.03f);
		if (input.ReportKeyState(keyMap['A']))
			parent->RotateLocalDeltaY(0.03f);
		parent->OrientateAxesToGlobalUp();
	}
}

void Controller::SetKeyMap(char keyToMap, char keyToMapTo)
{
	keyMap[keyToMap] = keyToMapTo;
}
