#pragma once
#include "myvector2.h"
#include <map>
#include "texture.h"
#include "noisegenerator.h"
#include "terrain.h"
#include "material.h"

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
	Material GetMaterial(int i, int j){return materials[i][j];}
	Material GetWaterMaterial(int i, int j){return waterMats[i][j];}
	int GetSize(){return numChunks;}
	Vector2 GetBase(int i , int j){return bases[i][j];}

private:
	static const int range = 500;
	static const int chunkSize = 1500;
	static const int numChunks = (2*range)/(chunkSize/2)+1;

	Vector2 bases[numChunks][numChunks];
	Material materials[numChunks][numChunks];
	Material waterMats[numChunks][numChunks];
	Material defaultGround;
	Material defaultWater;
	Terrain terrain;
	Water water;

	std::map<float, std::map<float, Texture> > terrainMap;	

	RenderManager* renderer;
	GameObject* camera;
	NoiseObject noise;

};

