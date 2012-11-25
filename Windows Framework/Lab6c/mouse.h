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
	void Message(UINT, LPARAM);
	void Update();
	bool RButton() {return rmouse;}
	bool LButton() {return lmouse;}
	bool OldRButton() {return lastrmouse;}
	bool OldLButton() {return lastlmouse;}
	bool rClick() {return rmouseclick;}
	bool lClick() {return lmouseclick;}
	bool lRelease() {return lrelease;}
	bool rRelease() {return rrelease;}
	int FrameDX(){return (x-Lockedx);}
	int FrameDY(){return (y-Lockedy);}
	int Location(axis);
	void SetLocked(int xl, int yl){Lockedx = xl;Lockedy=yl;x=xl;y=yl;}

private:
	int x, y;
	int oldx, oldy;
	int Lockedx, Lockedy;
	bool lmouse, rmouse;
	bool lastlmouse, lastrmouse;
	bool lmouseclick, rmouseclick;
	bool lrelease, rrelease;


	void UpdatePosition(LPARAM);
};