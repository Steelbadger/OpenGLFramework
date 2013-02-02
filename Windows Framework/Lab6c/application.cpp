#include "application.h"
#include <iostream>
#include <time.h>


Application::Application(void):
	myNoise(10, 200.0f, 0.45f, 40.0f, -1563.155f),
	ground(500.0f, myNoise, 3.0f)
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
	renderer.SetActiveCamera(*player.GetCamera());


	Mesh skyBox("inwardCube.obj", "skyboxseamless.tga");
	renderer.AddSkyBox(skyBox);
	int count = 0;
	int num = 10;
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < num; j++) {
			for (int k = 0; k < num; k++) {
				count++;
				StaticObject* curr = new StaticObject();
				curr->CreateAndAttachMesh("crate.obj", "crateDiffuse.tga");
				curr->SetLocation(i*3.0f, 20.0f+j*3.0f, k*3.0f);
				renderer.AddToRenderer(*curr->GetMesh());
			}
		}
	}
	testObject.CreateAndAttachMesh("crate.obj", "crateDiffuse.tga");
	testObject.SetLocation(100.0f, 40.0f, 100.0f);
	renderer.AddToRenderer(*testObject.GetMesh());

	cube = new CubeArray(1,1,1, "Crate.tga");

	double myTimer = clock();

	renderer.AddTerrainToRenderer(ground);

	myTimer = clock() - myTimer;
	int numTextureUnits;

	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &numTextureUnits);

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL 4.2 Initialized: " << 	GLEW_VERSION_4_2 << std::endl;
	std::cout << "Number of Texture Units: " << numTextureUnits << std::endl;
	std::cout << std::endl << "Time to Add Terrain To Renderer: " << myTimer/CLOCKS_PER_SEC << "s" << std::endl;

	lastTime = time(NULL);
	nbFrames = 0;
	wireframe = false;
	framerateLogging = false;
	culling = false;
	glCullFace(GL_BACK);

}

void Application::MainLoop()
{
	if (input.GetTimeSinceLastFrame() >= 10) {
		double currentTime = time(NULL);
		nbFrames++;
		if ( currentTime - lastTime >= 1.0 ){
			if (framerateLogging) {
				std::cout << 1000.0/double(nbFrames) << " ms/frame\t" << "(" << nbFrames << "FPS)" << std::endl;
			}
			nbFrames = 0;
			lastTime += 1.0;
		}

		GLfloat Light_Ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
		GLfloat Light_Diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
		GLfloat Light_Position[]= {500.0f, 1000.0f, 500.0f, 1.0f};

		glLoadIdentity();
		window.PrepareForDrawing();
		player.InputUpdate();

		testObject.RotateLocalDeltaZ(0.05f);

		if (input.ReportKeyState(VK_ADD)) {
			testObject.UniformScale(1.01f);
		}

		if (input.ReportKeyState(VK_SUBTRACT)) {
			testObject.UniformScale(0.99f);
		}

		player.CheckGroundCollision(myNoise);

		renderer.UpdateCamera();

		input.Update();

		if(input.ReportRMousePress()) {
			window.SetCursorToCentre();
			window.SetMouseLockedCentre();
		}

		if (input.ReportKeyPress('F')) {
			framerateLogging = !framerateLogging;
		}
		if (input.ReportKeyPress('C')) {
			culling = !culling;
			if (culling) {
				glEnable(GL_CULL_FACE);
				std::cout << "Backface Culling: ON" << std::endl;
			} else {
				glDisable(GL_CULL_FACE);
				std::cout << "Backface Culling: OFF" << std::endl;
			}

		}


		if (input.ReportKeyPress('N')) {
			std::cout << "Last Frame Took: " << input.GetTimeForLastFrame() << "ms" << std::endl;
		}

		if (input.ReportKeyPress('L')) {
			std::cout << "Position: (" << player.GetPosition().x << ", "<< player.GetPosition().y << ", " << player.GetPosition().z << std::endl;
			std::cout << "Forward Vector: (" << player.GetLocalZ().x << ", " << player.GetLocalZ().y << ", " << player.GetLocalZ().z << ")\t(Length: " << player.GetLocalZ().Length() << ")" << std::endl;
			std::cout << "Up Vector: (" << player.GetLocalY().x << ", " << player.GetLocalY().y << ", " << player.GetLocalY().z << ")\t(Length: " << player.GetLocalY().Length() << ")" << std::endl;
			std::cout << "Rotation Quaternion: (" << player.GetRotation().s << ", (" << player.GetRotation().x << ", " << player.GetRotation().y << ", " << player.GetRotation().z << "))" << std::endl;
			std::cout << "Rotation Angle: " << player.GetRotation().GetAngle() << "\tRotation Axis: (" << player.GetRotation().GetAxis().x << ", " << player.GetRotation().GetAxis().y << ", " << player.GetRotation().GetAxis().z << ")" << std::endl;
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


		glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT1, GL_AMBIENT,  Light_Ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE,  Light_Diffuse);
		glLightfv(GL_LIGHT1, GL_POSITION, Light_Position);
		glEnable(GL_LIGHT1);

		renderer.RenderAll();
	
		glTranslatef(-25.0f, 0.0f, -25.0f);
	
			for (int i = 0; i < 24; i++) {
				for (int j = 0; j < 24; j++) {
					glTranslatef(2.0f, 0.0f, 0.0f);
					//cube->Draw();
				}
				glTranslatef(-48.0f, 0.0f, 2.0f);
			}	

		window.FlipBuffers();
	}
}
