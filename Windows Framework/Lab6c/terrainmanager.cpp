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

	for(int i = 0; i < numChunks; i++) {
		for(int j = 0; j < numChunks; j++) {
			bases[i*numChunks+j] = Vector2(base.u + i*chunkSize, base.v + j * chunkSize);
			std::cout << "Generating at: (" << bases[i*numChunks+j].u << ", " << bases[i*numChunks+j].v << ")" << std::endl;
			terrainMap[bases[i*numChunks+j].u][bases[i*numChunks+j].v] = 
							Texture(Texture::DISPLACEMENT, heights.TBBGenerateHeightField(bases[i*numChunks+j].u, bases[i*numChunks+j].v, noise, chunkSize), 512);
			std::cout << "Applying Material..." << std::endl;
			defaultGround.ReplaceTexture(oldTex, terrainMap[bases[i*numChunks+j].u][bases[i*numChunks+j].v]);
			defaultWater.ReplaceTexture(oldTex, terrainMap[bases[i*numChunks+j].u][bases[i*numChunks+j].v]);
			oldTex = terrainMap[bases[i*numChunks+j].u][bases[i*numChunks+j].v];
			materials[i*numChunks+j] = defaultGround;
			waterMats[i*numChunks+j] = defaultWater;
			std::cout << "Done" << std::endl << std::endl;
		}
	}

	std::cout << "DONE TERRAIN INIT" << std::endl;


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
		//	updateTask->execute();

			tbb::task::enqueue(*updateTask);
		}
	}

	//Heightmap heights;
	//std::map<float, std::map<float, Texture> > oldTerrain;
	//std::map<float, std::map<float, Texture> > newTerrain;
	//terrainMutex.lock_read();
	//oldTerrain = terrainMap;
	//terrainMutex.unlock();
	//Texture oldTex;
	//Material materialsBuff[numChunks][numChunks];
	//Material waterMatsBuff[numChunks][numChunks];
	//Vector2 basesBuff[numChunks][numChunks];

	//for(int i = 0; i < numChunks; i++) {
	//	for(int j = 0; j < numChunks; j++) {
	//		basesBuff[i][j] = Vector2(base.u + i*chunkSize, base.v + j * chunkSize);
	//		if (oldTerrain[basesBuff[i][j].u].count(basesBuff[i][j].v) == 0) {
	//			oldTex = oldTerrain[basesBuff[i][j].u][basesBuff[i][j].v];
	//			std::cout << "Generating New Terrain Chunk: (" << basesBuff[i][j].u << ", " << basesBuff[i][j].v << ")" << std::endl;
	//			newTerrain[basesBuff[i][j].u][basesBuff[i][j].v] = 
	//						Texture(Texture::DISPLACEMENT, heights.TBBGenerateHeightField(basesBuff[i][j].u, basesBuff[i][j].v, noise, chunkSize), 512);
	//			std::cout << "Done" << std::endl;
	//		} else {
	//			oldTex = oldTerrain[basesBuff[i][j].u][basesBuff[i][j].v];
	//			newTerrain[basesBuff[i][j].u][basesBuff[i][j].v] = oldTex;
	//		}
	//		Material defaultGround = materials[i*numChunks+j];
	//		defaultGround.ReplaceTexture(Texture::DISPLACEMENT, newTerrain[basesBuff[i][j].u][basesBuff[i][j].v]);
	//		defaultWater.ReplaceTexture(Texture::DISPLACEMENT, newTerrain[basesBuff[i][j].u][basesBuff[i][j].v]);
	//		oldTex = newTerrain[basesBuff[i][j].u][basesBuff[i][j].v];
	//		materialsBuff[i][j] = defaultGround;
	//		waterMatsBuff[i][j] = defaultWater;
	//	}
	//}

	//newTerrain.swap(terrainMap);

	//for (int i = 0; i < numChunks;i++) {
	//	for (int j = 0; j < numChunks; j++) {
	//		materials[i*numChunks+j] = materialsBuff[i][j];
	//		waterMats[i*numChunks+j] = waterMatsBuff[i][j];
	//		bases[i*numChunks+j] = basesBuff[i][j];
	//	}
	//}	

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
