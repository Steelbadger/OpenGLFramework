#pragma once
#include <windows.h>
#include "mouse.h"
#include "singleton.h"



class Input : public CSingleton<Input>
{
public:
	Input(void);
	~Input(void);

	void Message(UINT, WPARAM, LPARAM);

	void Update();

	int GetMouseX() {return mouse.GetX();}
	int GetMouseY() {return mouse.GetY();}
	bool GetMouseR() {return mouse.RButton();}
	bool GetMouseL() {return mouse.LButton();}
	bool GetOldMouseR() {return mouse.OldRButton();}
	bool GetOldMouseL() {return mouse.OldLButton();}
	bool ReportKeyState(int key) {return keys[key];}
	bool ReportKeyState(char key) {return keys[key];}
	bool ReportKeyPress(int key) {return pressed[key];}
	bool ReportKeyPress(char key) {return pressed[key];}
	bool ReportKeyRelease(int key){return released[key];}
	bool ReportKeyRelease(char key){return released[key];}
	bool ReportLMousePress() {return mouse.lClick();}
	bool ReportRMousePress() {return mouse.rClick();}
	bool ReportLMouseRelease() {return mouse.lRelease();}
	bool ReportRMouseRelease() {return mouse.rRelease();}
	int GetMouseDX(){return mouse.FrameDX();}
	int GetMouseDY(){return mouse.FrameDY();}
	void SetMouseLocked(int x, int y){mouse.SetLocked(x, y);}
	int ReportMouseLocation(Mouse::axis Axis) {return mouse.Location(Axis);}
private:
	bool keys[256];
	bool pressed[256];
	bool released[256];
	bool oldkey[256];
	Mouse mouse;

	void KeyDown(UINT);
	void KeyUp(UINT);

};
