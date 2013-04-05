#pragma once
#include "myvector2.h"
#include <map>
#include "texture.h"
#include "noisegenerator.h"
#include "terrain.h"
#include "heightmap.h"
#include "material.h"
#include "TBB\task.h"
#include "TBB\spin_rw_mutex.h"
#include <iostream>


class RenderManager;
class GameObject;

static const int range = 1000;
static const int chunkSize = 600;
static const int numChunks = (2*range)/(chunkSize/2)-2;

class UpdateTask : public tbb::task
{
public:
	UpdateTask(): noise(16, 500.0f, 0.41f, 80.0f, 1.155f){};
	void Initialize(std::map<float, std::map<float, Texture> > &terrain, Material *mat, Material *wMat, Vector2 *bas, NoiseObject n, Material dground, Material dwater) {
		terrainMap = &terrain;
		materials = mat;
		waterMaterials = wMat;
		bases = bas;
		noise = n;
		defaultGround = dground;
		defaultWater = dwater;
	}

	void PassMutexes(tbb::spin_rw_mutex &material, tbb::spin_rw_mutex &water, tbb::spin_rw_mutex &basesm, tbb::spin_rw_mutex &terrainm) {
		materialMutex = material;
		waterMutex = water;
		basesMutex = basesm;
		terrainMutex = terrainm;
	}

	void SetupForExecute(Vector2 b) {
		base = b;
	}

	tbb::task* execute() {
		Heightmap heights;
		std::map<float, std::map<float, Texture> > oldTerrain;
		std::map<float, std::map<float, Texture> > newTerrain;
		terrainMutex.lock_read();
		oldTerrain = *terrainMap;
		terrainMutex.unlock();
		Texture oldTex;
		Material materialsBuff[numChunks][numChunks];
		Material waterMatsBuff[numChunks][numChunks];
		Vector2 basesBuff[numChunks][numChunks];

		for(int i = 0; i < numChunks; i++) {
			for(int j = 0; j < numChunks; j++) {
				basesBuff[i][j] = Vector2(base.u + i*chunkSize, base.v + j * chunkSize);
				if (oldTerrain[basesBuff[i][j].u].count(basesBuff[i][j].v) == 0) {
					oldTex = oldTerrain[basesBuff[i][j].u][basesBuff[i][j].v];
					std::cout << "Generating New Terrain Chunk: (" << basesBuff[i][j].u << ", " << basesBuff[i][j].v << ")" << std::endl;
					newTerrain[basesBuff[i][j].u][basesBuff[i][j].v] = 
								Texture(Texture::DISPLACEMENT, heights.TBBGenerateHeightField(basesBuff[i][j].u, basesBuff[i][j].v, noise, chunkSize), 512);
					std::cout << "Done" << std::endl;
				} else {
					oldTex = oldTerrain[basesBuff[i][j].u][basesBuff[i][j].v];
					newTerrain[basesBuff[i][j].u][basesBuff[i][j].v] = oldTex;
				}
				materialMutex.lock_read();
				Material defaultGround = materials[i*numChunks+j];
				materialMutex.unlock();
				defaultGround.ReplaceTexture(Texture::DISPLACEMENT, newTerrain[basesBuff[i][j].u][basesBuff[i][j].v]);
				defaultWater.ReplaceTexture(Texture::DISPLACEMENT, newTerrain[basesBuff[i][j].u][basesBuff[i][j].v]);
				oldTex = newTerrain[basesBuff[i][j].u][basesBuff[i][j].v];
				materialsBuff[i][j] = defaultGround;
				waterMatsBuff[i][j] = defaultWater;
			}
		}

		terrainMutex.lock();
		newTerrain.swap(*terrainMap);
		terrainMutex.unlock();

		materialMutex.lock();
		waterMutex.lock();
		basesMutex.lock();
		for (int i = 0; i < numChunks;i++) {
			for (int j = 0; j < numChunks; j++) {
				materials[i*numChunks+j] = materialsBuff[i][j];
				waterMaterials[i*numChunks+j] = waterMatsBuff[i][j];
				bases[i*numChunks+j] = basesBuff[i][j];
			}
		}	
		basesMutex.unlock();
		waterMutex.unlock();
		materialMutex.unlock();

		tbb::task::recycle_as_continuation();

		return NULL;
	}

private:
	Vector2 base;
	std::map<float, std::map<float, Texture> > *terrainMap;
	Material* materials;
	Material* waterMaterials;
	Vector2* bases;
	Material defaultGround;
	Material defaultWater;
	NoiseObject noise;

//  MUTEXES

	tbb::spin_rw_mutex materialMutex;
	tbb::spin_rw_mutex waterMutex;
	tbb::spin_rw_mutex basesMutex;
	tbb::spin_rw_mutex terrainMutex;
};




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
	int GetSize(){return numChunks;}
	Vector2 GetBase(int i , int j);

private:

	Vector2 bases[numChunks*numChunks];
	Material materials[numChunks*numChunks];
	Material waterMats[numChunks*numChunks];
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

};