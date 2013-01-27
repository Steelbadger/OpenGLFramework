#pragma once
//  Author - Ross Davies
//  This class creates a window and OpenGL render context
//  Additional functions to retrieve details on the window


#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <mmsystem.h>
#include "input.h"
#include <map>


class WindowWizard
{
public:
	WindowWizard(void);
	WindowWizard(LPSTR WindowName, int width, int height, HINSTANCE hInstance);
	WindowWizard(LPSTR WindowName, int width, int height, HINSTANCE hInstance, bool FullScreen, DWORD Style);
	void WindowCreate(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);

	~WindowWizard(void);

	void InitializeGraphics(GLdouble fov);
	void OnResize(int width, int height);
	void OnMove();
	void FlipBuffers();
	void PrepareForDrawing();

	int GetWidth(){return graphicsRect.right;}
	int GetHeight(){return graphicsRect.bottom;}
	int GetX(){return windowRect.left;};
	int GetY(){return windowRect.top;}

	void SetCursorToCentre();
	void SetMouseLockedCentre(){Input::GetSingleton().SetMouseLocked(gWidth/2,gHeight/2);}

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static WindowWizard* GetWindowReference(HWND hWnd);

	HDC GetHandleToDeviceContext(){return hdc;};
	HWND GetHandleToWindow(){return handleToWindow;}
	HGLRC GetRenderingContext(){return hrc;}

private:
	void InitializeOpenGL(GLdouble fov);
	void ResizeGLWindow(int width, int height);
	bool SetWindowPixelFormat(HDC hdc);

	HWND handleToWindow;
	HDC hdc;
	HGLRC hrc;			//hardware RENDERING CONTEXT
	WNDCLASS wcex;
	RECT graphicsRect;
	RECT windowRect;
	GLdouble fieldOfView;

	int gWidth, gHeight;
	static std::map<HWND,WindowWizard*>WindowMap;
	static int cxSource, cySource;

	static const int COLOUR_DEPTH = 16;
};

