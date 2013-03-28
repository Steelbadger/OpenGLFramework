#include "application.h"

#include "heightmap.h"
#include "lights.h"
#include "water.h"

#include <iostream>
#include <time.h>


Application::Application(void):
	gridSize(1500.0f),
	myNoise(12, 500.0f, 0.41f, 80.0f, -1563.155f),
	ground(gridSize, 15.0f),
	skybox("inwardCube.obj"),
	water(gridSize)
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glewInit();

	double myTimer = clock();

	myTimer = clock() - myTimer;
	int numTextureUnits;

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &numTextureUnits);

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "OpenGL 4.2 Initialized: " << 	GLEW_VERSION_4_2 << std::endl;
	std::cout << "Number of Texture Units: " << numTextureUnits << std::endl;
	std::cout << std::endl << "Time to Add Terrain To Renderer: " << myTimer/CLOCKS_PER_SEC << "s" << std::endl;

	myTimer = clock();

	player.SetLocation(50.0f, 30.0f, 50.0f);
	LightSource playerLight(LightSource::POINT);
	playerLight.SetColour(0.7f, 0.7f, 0.7f);
	playerLight.SetAmbient(0.0f);

	LightSource sunSource(LightSource::DIRECTIONAL);
	sunSource.SetColour(0.7f, 0.7f, 0.7f);
	sunSource.SetAmbient(0.2f);

	sunSource.SetParent(sunParent);
	player.SetCameraTargetWindow(&window);
	playerLight.SetParent(player);

	renderer.SetActiveCamera(*player.GetCamera());
	renderer.AddLight(sunSource);
	renderer.AddLight(playerLight);

	Material skyMat;
	skyMat.AddTexture(Texture(Texture::DIFFUSE, "skyboxseamless.tga"));
	skyMat.AddShader("skybox.fragmentshader");
	skyMat.AddShader("skybox.vertexshader");

	skybox = meshGenerator.InwardCubeSphere(5);

	skybox.AttachMaterial(skyMat);
	renderer.AddSkyBox(skybox);

	PrimitiveFactory prims;

	Terrain dummy(prims.Plane(1500.0f, 1500.0f, 100, 100));

	Material crateMatProc;
	crateMatProc.AddTexture(Texture(Texture::DIFFUSE, "crateDiffuseProc.tga"));
	crateMatProc.AddShader("default.fragmentshader");
	crateMatProc.AddShader("default.vertexshader");

	Material crateMat;
	crateMat.AddTexture(Texture(Texture::DIFFUSE, "crateDiffuse.tga"));
	crateMat.AddShader("default.fragmentshader");
	crateMat.AddShader("default.vertexshader");

	Mesh crateMesh("crate.obj");
	Mesh altCrate(meshGenerator.SubDivide(meshGenerator.UnitCube()));
	Mesh sphere(meshGenerator.LSphere(10, 10));

	altCrate.AttachMaterial(crateMatProc);
	sphere.AttachMaterial(crateMatProc);
	crateMesh.AttachMaterial(crateMat);

//	crateMesh.AttachMaterial(waterMat);

	int num = 5;
	for (int i = 0; i < num; i++) {
		for (int j = 0; j < num; j++) {
			for (int k = 0; k < num; k++) {
				StaticObject* curr = new StaticObject();
//				curr->AttachMesh(crateMesh);
//				curr->AttachMesh(sphere);
				curr->AttachMesh(altCrate);
				curr->SetLocation(i*3.0f, 20.0f+j*3.0f, k*3.0f);
				renderer.AddToRenderer(*curr->GetMesh());
			}
		}
	}
	Mesh newAlt(meshGenerator.CubeSphere(1));
	newAlt.AttachMaterial(crateMatProc);
//	testObject.AttachMesh(crateMesh);
	testObject.AttachMesh(newAlt);
	testObject.SetLocation(100.0f, 40.0f, 100.0f);
	renderer.AddToRenderer(*testObject.GetMesh());
	
	Material groundMat;
	groundMat.AddTexture(Texture(Texture::DIFFUSE, "grass.tga"));
	groundMat.AddTexture(Texture(Texture::DIFFUSE, "rockTexture.tga"));
	groundMat.AddShader("terrain.fragmentshader");
	groundMat.AddShader("terrain.vertexshader");
	groundMat.AddShader("terrain.tesscontrol");
	groundMat.AddShader("terrain.tessevaluation");


	Heightmap heights;
	Texture heightMap(Texture::DISPLACEMENT, heights.GenerateHeightField(0, 0, myNoise, gridSize), 2048);
	groundMat.AddTexture(heightMap);
	ground.AttachMaterial(groundMat);

	renderer.AddTerrainToRenderer(ground);

	myTimer = clock() - myTimer;

	std::cout << std::endl << "Time to Generate Heightmap: " << myTimer/CLOCKS_PER_SEC << "s" << std::endl;

	Material waterMat("waterMat");
	waterMat.AddShader("water.vertexshader");
	waterMat.AddShader("water.fragmentshader");
	waterMat.AddShader("water.tesscontrol");
	waterMat.AddShader("water.tessevaluation");
	waterMat.AddTexture(Texture(Texture::DIFFUSE, "seamlessWater.tga"));
	waterMat.AddTexture(heightMap);
	
	water.AttachMaterial(waterMat);

	renderer.AddWater(water);

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
		Debug();
		window.PrepareForDrawing();
		player.InputUpdate();

		testObject.RotateLocalDeltaZ(0.05f);
		testObject.RotateLocalDeltaY(0.025f);
		testObject.RotateLocalDeltaX(0.03f);

		if (input.ReportKeyState(VK_ADD)) {
			testObject.UniformScale(1.01f);
		}

		if (input.ReportKeyState(VK_SUBTRACT)) {
			testObject.UniformScale(0.99f);
		}

		player.CheckGroundCollision(myNoise);

		input.Update();

		if(input.ReportRMousePress()) {
			window.SetCursorToCentre();
			window.SetMouseLockedCentre();
		}

		if(input.ReportKeyState(VK_LEFT)) {
			sunParent.RotateDeltaX(0.01f);
		}

		if (input.ReportKeyState(VK_RIGHT)) {
			sunParent.RotateDeltaX(-0.01f);
		}

		if (input.ReportKeyPress(VK_ESCAPE))
			exit(0);

		renderer.RenderAll();

		window.FlipBuffers();
	}
}

void Application::Debug()
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
		std::cout << "Last Frame Took: " << input.GetTimeForLastFrame() << "s" << std::endl;
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
}
