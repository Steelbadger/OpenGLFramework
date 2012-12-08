#include "application.h"

Application::Application(void)
{
}


Application::~Application(void)
{
}

void Application::Initialize(HINSTANCE hInstance)
{
	float rot = 0.0;

	window.WindowCreate("OpenGL Framework", 1000, 500, (WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN), 0, hInstance);
	window.InitializeGraphics(45.0f);
	window.SetCursorToCentre();
	camera.SetFieldOfView(45.0f);
	camera.SetWindowSize(window.GetWidth(), window.GetHeight());
	camera.SetClipPlanes(1.0f, 150.0f);

	cube = new CubeArray(1,1,1, "Crate.tga");
	ground.Create();
}

void Application::MainLoop()
{

	GLfloat Light_Ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
	GLfloat Light_Diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat Light_Position[]= {3.0f, 3.0f, 0.0f, 1.0f};

	window.PrepareForDrawing();

	camera.Update();
	input.Update();

	if (input.ReportKeyState('W'))
		camera.MoveForward(-0.004f);
	if(input.ReportKeyState('S'))
		camera.MoveForward(0.004f);
	if(input.ReportKeyRelease('C'))
		camera.SetVectors(0,0,-6,0,0,0);
	if(input.ReportRMousePress()) {
		window.SetCursorToCentre();
		window.SetMouseLockedCentre();
	}


	if (input.GetMouseR()) {
		camera.RotateYaw(-(float)input.GetMouseDX()/1000.0);
		camera.RotatePitch((float)input.GetMouseDY()/1000.0);
		window.SetCursorToCentre();
		if (input.ReportKeyState('D'))
			camera.Strafe(-0.003f);
		if (input.ReportKeyState('A'))
			camera.Strafe(0.003f);
	} else {
		if (input.ReportKeyState('D'))
			camera.RotateYaw(-0.001f);
		if (input.ReportKeyState('A'))
			camera.RotateYaw(0.001f);
	}
	if (input.CheckMouseWheel()) {
		camera.Zoom(input.GetMouseWheelDelta());
	}

	if (input.ReportKeyPress(VK_ESCAPE))
		exit(0);

	Light_Position[0] = camera.GetCameraX();
	Light_Position[1] = camera.GetCameraY();
	Light_Position[2] = camera.GetCameraZ();

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_AMBIENT,  Light_Ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  Light_Diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, Light_Position);
	glEnable(GL_LIGHT1);

	ground.Draw();

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			glTranslatef(2.0f, 0.0f, 0.0f);
			cube->Draw();
		}
		glTranslatef(-10.0f, 0.0f, 2.0f);
	}
	
	
	window.FlipBuffers();
}
