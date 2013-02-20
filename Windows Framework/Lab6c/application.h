#pragma once
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include "glew.h"
#include "Terrain.h"
#include "mywindow.h"
#include "player.h"
#include "renderer.h"
#include "staticobject.h"
#include "noisegenerator.h"

class Application
{
public:
	Application(void);
	~Application(void);

	void Initialize(HINSTANCE hInstance);
	void MainLoop();

	void Debug();

private:
	WindowWizard window;
	Player player;

	double lastTime;
	int nbFrames;
	double currentTime;
	bool wireframe;
	bool framerateLogging;
	bool culling;

	const int gridSize;
	NoiseObject myNoise;
	Terrain ground;
	RenderManager renderer;
	GameObject sunParent;

	StaticObject testObject;
};

