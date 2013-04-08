#include "terrainmanager.h"
#include "renderer.h"
#include "terrain.h"
#include "gameobject.h"
#include "heightmap.h"
#include "material.h"

#include <iostream>




TerrainManager::TerrainManager(void) : noise(16, 500.0f, 0.41f, 80.0f, 1.155f),	terrain(chunkSize, 15.0f), water(chunkSize)
{
}


TerrainManager::~TerrainManager(void)
{
}


void TerrainManager::Initialize(RenderManager &r, NoiseObject n)
{
	std::cout << "Terrain Manager Initialising!" << std::endl;

	renderer = &r;
	camera = renderer->GetCameraParent();
	noise = n;
	updateTask = NULL;

	Vector2 base = (camera->GetPosition().xz() - range)/chunkSize;
	std::cout << "Range Lower Extent: (" << base.u << ", " << base.v << ")" << std::endl;
	base.Floor();
	base = base * chunkSize;
	Heightmap heights;

	std::cout << "Base of Terrain Map at: (" << base.u << ", " << base.v << ")" << std::endl;
	std::cout << "Beginning Terrain Generation Loop" << std::endl;

	defaultGround.AddTexture(Texture(Texture::DIFFUSE, "grass.tga"));
	defaultGround.AddTexture(Texture(Texture::DIFFUSE, "rockTexture.tga"));
	defaultGround.AddShader("terrain.fragmentshader");
	defaultGround.AddShader("terrain.vertexshader");
	defaultGround.AddShader("terrain.tesscontrol");
	defaultGround.AddShader("terrain.tessevaluation");
	defaultGround.Compile();

	defaultWater.AddShader("water.vertexshader");
	defaultWater.AddShader("water.fragmentshader");
	defaultWater.AddShader("water.tesscontrol");
	defaultWater.AddShader("water.tessevaluation");
	defaultWater.AddTexture(Texture(Texture::DIFFUSE, "seamlessWater.tga"));
	defaultWater.Compile();
	Texture oldTex;

	double bigTimer = clock();
	double timer = clock();

	//bases = new Vector2[numChunks*numChunks];
	//materials = new Material[numChunks*numChunks];
	//waterMats = new Material[numChunks*numChunks];

	for(int i = 0; i < numChunks; i++) {
		for(int j = 0; j < numChunks; j++) {
			bases[i*numChunks+j] = Vector2(base.u + i*chunkSize, base.v + j * chunkSize);
			std::cout << "Generating at: (" << bases[i*numChunks+j].u << ", " << bases[i*numChunks+j].v << ")" << std::endl;
			timer = clock();
			terrainMap[bases[i*numChunks+j].u][bases[i*numChunks+j].v] = 
							Texture(Texture::DISPLACEMENT, heights.TBBSIMDGenerateHeightField(bases[i*numChunks+j].u, bases[i*numChunks+j].v, noise, chunkSize), 1024);
			std::cout << "Applying Material..." << std::endl;
			defaultGround.ReplaceTexture(oldTex, terrainMap[bases[i*numChunks+j].u][bases[i*numChunks+j].v]);
			defaultWater.ReplaceTexture(oldTex, terrainMap[bases[i*numChunks+j].u][bases[i*numChunks+j].v]);
			oldTex = terrainMap[bases[i*numChunks+j].u][bases[i*numChunks+j].v];
			materials[i*numChunks+j] = defaultGround;
			waterMats[i*numChunks+j] = defaultWater;
			timer = clock()-timer;
			std::cout << "Done in " << timer/CLOCKS_PER_SEC << "s" << std::endl;
		}
	}

	bigTimer = clock()-bigTimer;
	
	std::cout << "DONE TERRAIN INIT (" << bigTimer/CLOCKS_PER_SEC << "s)" << std::endl;


}

void TerrainManager::Update()
{
	Vector2 base = (camera->GetPosition().xz() - range)/chunkSize;
	base.Floor();
	base = base * chunkSize;

	if (updateTask != NULL) {
		 if (updateTask->state() != tbb::task::executing) {
				updateTask = new(tbb::task::allocate_root())UpdateTask;
				updateTask->Initialize(terrainMap, materials, waterMats, bases, noise, defaultGround, defaultWater);
				updateTask->PassMutexes(materialMutex, waterMutex, basesMutex, terrainMutex);
				updateTask->SetupForExecute(base);

				tbb::task::enqueue(*updateTask);
		 }
	} else {
		updateTask = new(tbb::task::allocate_root())UpdateTask;
		updateTask->Initialize(terrainMap, materials, waterMats, bases, noise, defaultGround, defaultWater);
		updateTask->PassMutexes(materialMutex, waterMutex, basesMutex, terrainMutex);
		updateTask->SetupForExecute(base);

		tbb::task::enqueue(*updateTask);
	}
}

Material TerrainManager::GetMaterial(int x , int y)
{
	materialMutex.lock_read();
	Material output = materials[x*numChunks+y];
	materialMutex.unlock();
	return output;
}

Material TerrainManager::GetWaterMaterial(int x, int y)
{
	waterMutex.lock_read();
	Material output = waterMats[x*numChunks+y];
	waterMutex.unlock();
	return output;
}

Vector2 TerrainManager::GetBase(int x, int y)
{
	basesMutex.lock_read();
	Vector2 output = bases[x*numChunks+y];
	basesMutex.unlock();
	return output;
}
