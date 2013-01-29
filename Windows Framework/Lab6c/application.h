#pragma once
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include "glew.h"
#include "Primitives.h"
#include "Terrain.h"
#include "mywindow.h"
#include "player.h"
#include "renderer.h"

class Application
{
public:
	Application(void);
	~Application(void);

	void Initialize(HINSTANCE hInstance);
	void MainLoop();

private:
	WindowWizard window;
	Player player;

	double lastTime;
	int nbFrames;
	double currentTime;
	bool wireframe;
	bool framerateLogging;
	bool culling;

	CubeArray *cube;
	Terrain ground;
	RenderManager renderer;
};

