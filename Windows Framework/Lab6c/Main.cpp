#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include "application.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int nCmdShow)			
{	
    MSG msg;
	Application myApp;

	myApp.Initialize(hInstance);

	while (true) {							
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
		    if (msg.message==WM_QUIT)
				break;
			TranslateMessage (&msg);							
			DispatchMessage (&msg);
		} else {	
			myApp.MainLoop();
		}
    }
	return msg.wParam ;										
}