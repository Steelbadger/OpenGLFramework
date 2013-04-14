#pragma once
#include "barrier.h"
#include "myvector2.h"
#include <map>
#include "texture.h"
#include "noisegenerator.h"
#include "terrainupdatetask.h"
#include "terrain.h"
#include "heightmap.h"
#include "material.h"
#include "TBB\task.h"
#include "TBB\spin_rw_mutex.h"
#include "TBB\atomic.h"
#include <iostream>
#include <time.h>


class RenderManager;
class GameObject;

class TerrainManager
{
public:
	TerrainManager(void);
	~TerrainManager(void);
	void Initialize(RenderManager &renderer, NoiseObject n);
	void Update();
	Terrain GetTerrainMesh(){return terrain;}
	Water GetWaterMesh(){return water;}
	Material GetMaterial(int i, int j);
	Material GetWaterMaterial(int i, int j);
	int GetSize(){return NUMCHUNKS;}
	Vector2 GetBase(int i , int j);

private:

	Vector2 bases[NUMCHUNKS*NUMCHUNKS];
	Material materials[NUMCHUNKS*NUMCHUNKS];
	Material waterMats[NUMCHUNKS*NUMCHUNKS];

	Material defaultGround;
	Material defaultWater;
	Terrain terrain;
	Water water;

	std::map<float, std::map<float, Texture> > terrainMap;	

	RenderManager* renderer;
	GameObject* camera;
	NoiseObject noise;

	UpdateTask* updateTask;

//  MUTEXES

	tbb::spin_rw_mutex materialMutex;
	tbb::spin_rw_mutex waterMutex;
	tbb::spin_rw_mutex basesMutex;
	tbb::spin_rw_mutex terrainMutex;

//  BARRIER
	Barrier barrier;

};