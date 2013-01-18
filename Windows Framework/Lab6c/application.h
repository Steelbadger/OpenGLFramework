#pragma once
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include "Primitives.h"
#include "mesh.h"
#include "mywindow.h"
#include "camera.h"
#include "player.h"

class Application
{
public:
	Application(void);
	~Application(void);

	void Initialize(HINSTANCE hInstance);
	void MainLoop();

private:
	WindowWizard window;
	//Camera camera;
	Player player;

	double lastTime;
	int nbFrames;
	double currentTime;

	CubeArray *cube;
	Mesh ground;
};

