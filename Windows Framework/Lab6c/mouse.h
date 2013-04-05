#pragma once
#include <windows.h>

class Mouse
{
public:

	enum axis {X, Y};

	Mouse(void);
	~Mouse(void);

	int GetX() {return x;}
	int GetY() {return y;}
	void Message(UINT, WPARAM, LPARAM);
	void Update();
	bool RButton() {return rmouse;}
	bool LButton() {return lmouse;}
	bool MButton() {return mmouse;}
	bool OldRButton() {return lastrmouse;}
	bool OldLButton() {return lastlmouse;}
	bool OldMButton() {return lastmmouse;}
	bool rClick() {return rmouseclick;}
	bool lClick() {return lmouseclick;}
	bool mClick() {return mmouseclick;}
	bool lRelease() {return lrelease;}
	bool rRelease() {return rrelease;}
	bool mRelease() {return mrelease;}
	int FrameDX(){return (x-Lockedx);}
	int FrameDY(){return (y-Lockedy);}
	int Location(axis);
	void SetLocked(int xl, int yl){Lockedx = xl;Lockedy=yl;x=xl;y=yl;}
	int GetWheelDelta(){return wheelDelta;}
	bool CheckWheelMoved(){return wheelWasMoved;}

private:
	int x, y;
	int oldx, oldy;
	int Lockedx, Lockedy;
	bool lmouse, rmouse, mmouse;
	bool lastlmouse, lastrmouse, lastmmouse;
	bool lmouseclick, rmouseclick, mmouseclick;
	bool lrelease, rrelease, mrelease;
	bool wheelMoved;
	bool wheelWasMoved;
	int wheelDelta;


	void UpdatePosition(LPARAM);
};