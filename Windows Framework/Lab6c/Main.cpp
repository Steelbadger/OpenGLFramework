#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "input.h"
#include "Primitives.h"
#include "mywindow.h"
#include "camera.h"


GLfloat Light_Ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
GLfloat Light_Diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat Light_Position[]= {3.0f, 3.0f, 0.0f, 1.0f};


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int nCmdShow)			
{	

    MSG msg;
	Input input;
	Camera myCam;
	float rot = 0.0;

	WindowWizard MyWindow("My Window", 1000, 500, hInstance, false, 0);
	MyWindow.InitializeGraphics(45.0f);
	MyWindow.SetCursorToCentre();
	
	Cube cube(0,0,0,1,1,1, "Crate.tga");

	while (true)					
    {							
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
		    if (msg.message==WM_QUIT)
				break;
			TranslateMessage (&msg);							
			DispatchMessage (&msg);
		}

		else
		{	
			MyWindow.PrepareForDrawing();

			myCam.Update();
			input.Update();

			if (input.ReportKeyState('W'))
				myCam.MoveForward(-0.004f);
			if(input.ReportKeyState('S'))
				myCam.MoveForward(0.004f);
			if(input.ReportKeyRelease('C'))
				myCam.SetVectors(0,0,-6,0,0,0);
			if(input.ReportRMousePress()) {
				MyWindow.SetCursorToCentre();
				MyWindow.SetMouseLockedCentre();
			}


			if (input.GetMouseR()) {
				myCam.RotateYaw(-(float)input.GetMouseDX()/1000.0);
				myCam.RotatePitch((float)input.GetMouseDY()/1000.0);
				MyWindow.SetCursorToCentre();
				if (input.ReportKeyState('D'))
					myCam.Strafe(-0.003f);
				if (input.ReportKeyState('A'))
					myCam.Strafe(0.003f);
			} else {
				if (input.ReportKeyState('D'))
					myCam.RotateYaw(-0.001f);
				if (input.ReportKeyState('A'))
					myCam.RotateYaw(0.001f);
			}

			if (input.ReportKeyPress(VK_ESCAPE))
				exit(0);

			Light_Position[0] = myCam.GetCameraX();
			Light_Position[1] = myCam.GetCameraY();
			Light_Position[2] = myCam.GetCameraZ();

			glEnable(GL_LIGHTING);
			glLightfv(GL_LIGHT1, GL_AMBIENT,  Light_Ambient);
			glLightfv(GL_LIGHT1, GL_DIFFUSE,  Light_Diffuse);
			glLightfv(GL_LIGHT1, GL_POSITION, Light_Position);
			glEnable(GL_LIGHT1);

			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 5; j++) {
					glTranslatef(2.0f, 0.0f, 0.0f);
					glBegin (GL_QUADS);//Begin drawing state
					cube.Draw();
					glEnd();
				}
				glTranslatef(-10.0f, 2.0f, 0.0f);
			}
	
			MyWindow.FlipBuffers();
		}
    }

	return msg.wParam ;										
}