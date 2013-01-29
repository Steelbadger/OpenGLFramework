#include "controller.h"
#include "input.h"
#include "gameobject.h"


Controller::Controller(GameObject* p):
	parent(p),
	sensitivity(50.0f)
{
}


Controller::~Controller(void)
{
}

void Controller::CheckInputAndAct()
{
	if (input.ReportKeyState('W'))
		parent->MoveLocalDeltaZ(sensitivity*input.GetTimeForLastFrame());
	if(input.ReportKeyState('S'))
		parent->MoveLocalDeltaZ(-sensitivity*input.GetTimeForLastFrame());

	if (input.ReportKeyPress(VK_SPACE) && jumping == false) {
		parent->GetRigidbody()->SetVelocity(Vector4(0.0f, sensitivity, 0.0f, 1.0f));
		jumping = true;
	}


	if (input.GetMouseR()) {
		parent->RotateLocalDeltaY(-(float)input.GetMouseDX()/1000.0);
		parent->RotateLocalDeltaX(-(float)input.GetMouseDY()/1000.0);
		if (input.ReportKeyState('D'))
			parent->MoveLocalDeltaX(sensitivity*input.GetTimeForLastFrame());
		if (input.ReportKeyState('A'))
			parent->MoveLocalDeltaX(-sensitivity*input.GetTimeForLastFrame());
		parent->OrientateAxesToGlobalUp();
	} else {
		if (input.ReportKeyState('D'))
			parent->RotateLocalDeltaY(-sensitivity*input.GetTimeForLastFrame()/5);
		if (input.ReportKeyState('A'))
			parent->RotateLocalDeltaY(sensitivity*input.GetTimeForLastFrame()/5);
		parent->OrientateAxesToGlobalUp();
	}
}