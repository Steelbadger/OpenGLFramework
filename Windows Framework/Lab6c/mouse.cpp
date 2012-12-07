#ifndef _WINDOWS_H_DEFINED_
#define _WINDOWS_H_DEFINED_
#include <windows.h>
#include <WindowsX.h>
#endif
#include "mouse.h"
#include <math.h>
#include <stdio.h>

Mouse::Mouse(void)
{
	x = 0;
	y = 0;
	oldx = 0;
	oldy = 0;
	rmouse = false;
	lmouse = false;
}


Mouse::~Mouse(void)
{
}

void Mouse::UpdatePosition(LPARAM lParam)
{
	oldx = x;
	oldy = y;
	x = GET_X_LPARAM(lParam)+8; 
	y = GET_Y_LPARAM(lParam)+30; 
}

void Mouse::Message(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_MOUSEMOVE:
			UpdatePosition(lParam);
			break;
		case WM_MOUSEWHEEL:
			wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			wheelMoved = true;
			break;
		case WM_LBUTTONDOWN:
			lmouse = true;
			break;	
		case WM_LBUTTONUP:
			lmouse = false;
			break;
		case WM_RBUTTONDOWN:
			rmouse = true;
			break;
		case WM_RBUTTONUP:
			rmouse = false;
			break;
	}
}

void Mouse::Update()
{
	lmouseclick = false;
	rmouseclick = false;
	lrelease = false;
	rrelease = false;
	wheelWasMoved = wheelMoved;
	if (wheelMoved == false) {
		wheelDelta = 0;
	}
	wheelMoved = false;
	if (lmouse == true && lastlmouse == false)
	{
		lmouseclick = true;
	}
	if (rmouse == true && lastrmouse == false)
	{
		rmouseclick = true;
	}
	if (lmouse == false && lastlmouse == true)
	{
		lrelease = true;
	}
	if (rmouse == false && lastrmouse == true)
	{
		rrelease = true;
	}
	lastlmouse = lmouse;
	lastrmouse = rmouse;
}

int Mouse::Location(axis Axis)
{
	if (Axis == X)
		return x;
	else
		return y;
}