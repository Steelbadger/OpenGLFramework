#include "input.h"
#include "mouse.h"
#include <string.h>
#include <windows.h>


Input ourInputSingleton;

Input::Input(void)
{
	for (int i = 255; i >= 0; i--)
	{
		oldkey[i] = false;
		keys[i] = false;
		pressed[i] = false;
		released[i] = false;
	}
}


Input::~Input(void)
{
}

void Input::Message(UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
		case WM_SIZE:
			windowResized = true;
			break;
		case WM_KEYDOWN:
			KeyDown(wParam);
			break;

		case WM_KEYUP:
			KeyUp(wParam);
			break;

		case WM_MOUSEMOVE:
		case WM_MOUSEWHEEL:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			mouse.Message(message, wParam, lParam);
			break;
	}

}

void Input::KeyDown(UINT wParam)
{
	oldkey[wParam] = keys[wParam];
	keys[wParam]=true;
}

void Input::KeyUp(UINT wParam)
{
	oldkey[wParam] = keys[wParam];
	keys[wParam]=false;	
}

void Input::Update()
{
	windowResized = false;
	for (int i = 256; i > 0; i--)
	{
		if (keys[i-1] == true && oldkey[i-1] == false)
			pressed[i-1] = true;
		else
			pressed[i-1] = false;
	}
	for (int i = 256; i > 0; i--)
	{
		if (keys[i-1] == false && oldkey[i-1] == true)
			released[i-1] = true;
		else
			released[i-1] = false;
	}
	memcpy(oldkey, keys, sizeof(keys));
	mouse.Update();
}