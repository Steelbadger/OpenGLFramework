#pragma once
#include "myvector2.h"

class RenderManager;
class Terrain;
class GameObject;

class TerrainManager
{
public:
	TerrainManager(void);
	~TerrainManager(void);
	void Initialize(RenderManager &renderer);
	void Update();

private:
	static const int range = 500;
	static const int chunkSize = 1500;
	static const int numChunks = 2;

	Terrain* terrain[numChunks][numChunks];
	Vector2 bases[numChunks][numChunks];
	

	RenderManager* renderer;
	GameObject* camera;

};

