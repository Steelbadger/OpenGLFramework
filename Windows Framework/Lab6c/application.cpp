#include "application.h"
#include <iostream>
#include <time.h>

Application::Application(void):
	ground(1500.0f)
{
}


Application::~Application(void)
{
}

void Application::Initialize(HINSTANCE hInstance)
{
	srand(51816);
	float rot = 0.0;
	window.WindowCreate("OpenGL Framework", 1000, 500, (WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN), 0, hInstance);
	window.InitializeGraphics(45.0f);
	window.SetCursorToCentre();

	glewInit();
	player.SetLocation(50.0f, 30.0f, 50.0f);

	player.SetCameraTargetWindow(&window);

	player.InitSkyBox();
	cube = new CubeArray(1,1,1, "Crate.tga");

	double myTimer = clock();

	ground.Create();

	myTimer = clock() - myTimer;

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL 4.2 Initialized: " << 	GLEW_VERSION_4_2 << std::endl;
	std::cout << std::endl << "Time to Initialize: " << myTimer/CLOCKS_PER_SEC << "s" << std::endl;

	lastTime = time(NULL);
	nbFrames = 0;
	wireframe = false;
	framerateLogging = false;

}

void Application::MainLoop()
{
	double currentTime = time(NULL);
	nbFrames++;
	if ( currentTime - lastTime >= 1.0 ){
		if (framerateLogging) {
			std::cout << 1000.0/double(nbFrames) << " ms/frame\t" << "(" << nbFrames << "FPS)" << std::endl;
		}
		nbFrames = 0;
		lastTime += 1.0;
	}

	GLfloat Light_Ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat Light_Diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat Light_Position[]= {3.0f, 3.0f, 0.0f, 1.0f};

	glLoadIdentity();
	window.PrepareForDrawing();
	player.InputUpdate();



	player.CheckGroundCollision(ground);

	player.Update();

	input.Update();

	if(input.ReportRMousePress()) {
		window.SetCursorToCentre();
		window.SetMouseLockedCentre();
	}

	if (input.ReportKeyPress('F')) {
		framerateLogging = !framerateLogging;
	}

	if (input.ReportKeyPress('N')) {
		std::cout << "Last Frame Took: " << input.GetTimeForLastFrame() << "s" << std::endl;
	}

	if (input.ReportKeyPress('L')) {
		std::cout << "Position: (" << player.GetPosition().x << ", "<< player.GetPosition().y << ", " << player.GetPosition().z << ")\t";
		std::cout << "Forward Vector: (" << player.GetLocalZ().x << ", " << player.GetLocalZ().y << ", " << player.GetLocalZ().z << ")" << std::endl;
	}

	if (input.ReportKeyPress('I')) {
		wireframe = !wireframe;
		if (wireframe) {
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			std::cout << "WireFrame Mode: ON" << std::endl;
		} else {
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			std::cout << "WireFrame Mode: OFF" << std::endl;
		}
	}

	if (input.ReportKeyPress(VK_ESCAPE))
		exit(0);

	Light_Position[0] = player.GetPosition().x;
	Light_Position[1] = player.GetPosition().y;
	Light_Position[2] = player.GetPosition().z;

//	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_AMBIENT,  Light_Ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  Light_Diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, Light_Position);
	glEnable(GL_LIGHT1);

	player.DrawSkyBox();
	glClear(GL_DEPTH_BUFFER_BIT);
	ground.Draw();
	
	glTranslatef(-25.0f, 0.0f, -25.0f);
	
		for (int i = 0; i < 24; i++) {
			for (int j = 0; j < 24; j++) {
				glTranslatef(2.0f, 0.0f, 0.0f);
				cube->Draw();
			}
			glTranslatef(-48.0f, 0.0f, 2.0f);
		}	

	window.FlipBuffers();
}
