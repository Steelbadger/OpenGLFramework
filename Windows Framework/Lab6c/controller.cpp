#include "controller.h"
#include "input.h"
#include "gameobject.h"


Controller::Controller(GameObject* p):
	parent(p),
	sensitivity(0.5f)
{
}


Controller::~Controller(void)
{
}

void Controller::CheckInputAndAct()
{
	if (input.ReportKeyState('W'))
		parent->MoveLocalDeltaZ(sensitivity);
	if(input.ReportKeyState('S'))
		parent->MoveLocalDeltaZ(-sensitivity);

	if (input.ReportKeyPress(VK_SPACE) && jumping == false) {
		parent->GetRigidbody()->SetVelocity(Vector4(0.0f, sensitivity, 0.0f, 1.0f));
		jumping = true;
	}


	if (input.GetMouseR()) {
		parent->RotateLocalDeltaY(-(float)input.GetMouseDX()/1000.0);
		parent->RotateLocalDeltaX(-(float)input.GetMouseDY()/1000.0);
		if (input.ReportKeyState('D'))
			parent->MoveLocalDeltaX(sensitivity);
		if (input.ReportKeyState('A'))
			parent->MoveLocalDeltaX(-sensitivity);
		parent->OrientateAxesToGlobalUp();
	} else {
		if (input.ReportKeyState('D'))
			parent->RotateLocalDeltaY(-sensitivity/5);
		if (input.ReportKeyState('A'))
			parent->RotateLocalDeltaY(sensitivity/5);
		parent->OrientateAxesToGlobalUp();
	}
}