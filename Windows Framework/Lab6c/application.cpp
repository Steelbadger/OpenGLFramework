#include "application.h"
#include <iostream>
#include <time.h>

Application::Application(void)
{
}


Application::~Application(void)
{
}

void Application::Initialize(HINSTANCE hInstance)
{
	srand(unsigned int("herpdederp"));
	float rot = 0.0;
	window.WindowCreate("OpenGL Framework", 1000, 500, (WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN), 0, hInstance);
	window.InitializeGraphics(45.0f);
	window.SetCursorToCentre();
	player.SetLocation(50.0f, 30.0f, 50.0f);

	player.SetCameraTargetWindow(&window);

	cube = new CubeArray(1,1,1, "Crate.tga");
	ground.Create();
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	lastTime = time(NULL);
	nbFrames = 0;

}

void Application::MainLoop()
{
     double currentTime = time(NULL);
     nbFrames++;
     if ( currentTime - lastTime >= 1.0 ){
		 std::cout << 1000.0/double(nbFrames) << " ms/frame\t" << "(" << nbFrames << "FPS)" << std::endl;
         nbFrames = 0;
         lastTime += 1.0;
     }

	GLfloat Light_Ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat Light_Diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat Light_Position[]= {3.0f, 3.0f, 0.0f, 1.0f};
	if (player.GetPosition().y < 2.0f+ground.GetHeight(player.GetPosition().x, player.GetPosition().z) || player.GetPosition().y > 2.0f+ground.GetHeight(player.GetPosition().x, player.GetPosition().z)) {
		player.SetLocation(player.GetPosition().x, 2.0f+ground.GetHeight(player.GetPosition().x, player.GetPosition().z), player.GetPosition().z);
	}


	window.PrepareForDrawing();

	player.Update();
	if (player.GetPosition().y < 2.0f+ground.GetHeight(player.GetPosition().x, player.GetPosition().z) || player.GetPosition().y > 2.0f+ground.GetHeight(player.GetPosition().x, player.GetPosition().z)) {
		player.SetLocation(player.GetPosition().x, 2.0f+ground.GetHeight(player.GetPosition().x, player.GetPosition().z), player.GetPosition().z);
	}

	input.Update();

	if(input.ReportRMousePress()) {
		window.SetCursorToCentre();
		window.SetMouseLockedCentre();
	}

	if (input.ReportKeyPress(VK_ESCAPE))
		exit(0);

	Light_Position[0] = player.GetPosition().x;
	Light_Position[1] = player.GetPosition().y;
	Light_Position[2] = player.GetPosition().z;

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_AMBIENT,  Light_Ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  Light_Diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, Light_Position);
	glEnable(GL_LIGHT1);

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
