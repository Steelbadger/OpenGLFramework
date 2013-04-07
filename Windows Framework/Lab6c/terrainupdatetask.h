#pragma once

#include "myvector2.h"
#include <map>
#include "texture.h"
#include "noisegenerator.h"
#include "heightmap.h"
#include "material.h"
#include "TBB\task.h"
#include "TBB\spin_rw_mutex.h"
#include "terrainmanager.h"
#include <iostream>


class UpdateTask : public tbb::task
{
public:
	UpdateTask(): noise(16, 200.0f, 0.43f, 80.0f, 1.155f){};
	void Initialize(std::map<float, std::map<float, Texture> > &terrain, Material &mat, Material &wMat, Vector2 &bas, NoiseObject n) {
		terrainMap = &terrain;
		materials = &mat;
		waterMaterials = &wMat;
		bases = &bas;
		noise = n;
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
		Material materialsBuff[TerrainManager::numChunks][TerrainManager::numChunks];
		Material waterMatsBuff[TerrainManager::numChunks][TerrainManager::numChunks];
		Vector2 basesBuff[TerrainManager::numChunks][TerrainManager::numChunks];

		for(int i = 0; i < TerrainManager::numChunks; i++) {
			for(int j = 0; j < TerrainManager::numChunks; j++) {
				basesBuff[i][j] = Vector2(base.u + i*TerrainManager::chunkSize, base.v + j * TerrainManager::chunkSize);
				if (oldTerrain[basesBuff[i][j].u].count(basesBuff[i][j].v) == 0) {
					oldTex = oldTerrain[basesBuff[i][j].u][basesBuff[i][j].v];
					std::cout << "Generating New Terrain Chunk: (" << basesBuff[i][j].u << ", " << basesBuff[i][j].v << ")" << std::endl;
					newTerrain[basesBuff[i][j].u][basesBuff[i][j].v] = 
								Texture(Texture::DISPLACEMENT, heights.TBBGenerateHeightField(basesBuff[i][j].u, basesBuff[i][j].v, noise, TerrainManager::chunkSize), 512);
					std::cout << "Done" << std::endl;
				} else {
					oldTex = oldTerrain[basesBuff[i][j].u][basesBuff[i][j].v];
					newTerrain[basesBuff[i][j].u][basesBuff[i][j].v] = oldTex;
				}
				materialMutex.lock_read();
				Material defaultGround = materials[i*TerrainManager::numChunks+j];
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

		for (int i = 0; i < TerrainManager::numChunks;i++) {
			for (int j = 0; j < TerrainManager::numChunks; j++) {
				materials[i*TerrainManager::numChunks+j] = materialsBuff[i][j];
				waterMaterials[i*TerrainManager::numChunks+j] = waterMatsBuff[i][j];
				bases[i*TerrainManager::numChunks+j] = basesBuff[i][j];
			}
		}	
		basesMutex.unlock();
		waterMutex.unlock();
		materialMutex.unlock();

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
