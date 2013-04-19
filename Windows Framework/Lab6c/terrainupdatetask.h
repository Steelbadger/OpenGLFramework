#pragma once
#include "barrier.h"
#include "myvector2.h"
#include <map>
#include "texture.h"
#include "noisegenerator.h"
#include "terrain.h"
#include "heightmap.h"
#include "material.h"
#include "TBB\task.h"
#include "TBB\spin_rw_mutex.h"
#include "TBB\atomic.h"
#include <iostream>
#include <time.h>

static const int RANGE = 1000;
static const int CHUNKSIZE = 600;
static const int NUMCHUNKS = (2*RANGE)/(CHUNKSIZE/2)-2;


class UpdateTask : public tbb::task
{
public:
	UpdateTask(): noise(16, 200.0f, 0.43f, 80.0f, 1.155f){};
	void Initialize(std::map<float, std::map<float, Texture> > &terrain, 
					Material *mat, Material *wMat, Vector2 *bas, NoiseObject n, 
					Material dground, Material dwater) {
		terrainMap = &terrain;
		materials = mat;
		waterMaterials = wMat;
		bases = bas;
		noise = n;
		defaultGround = dground;
		defaultWater = dwater;
	}

	~UpdateTask() {
		(*threadCounter)--;
	}

	void PassMutexes(tbb::spin_rw_mutex &material, tbb::spin_rw_mutex &water, tbb::spin_rw_mutex &basesm, tbb::spin_rw_mutex &terrainm, tbb::atomic<int> &tcounter) {
		materialMutex = material;
		waterMutex = water;
		basesMutex = basesm;
		terrainMutex = terrainm;
		threadCounter = &tcounter;
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
		Material materialsBuff[NUMCHUNKS*NUMCHUNKS];
		Material waterMatsBuff[NUMCHUNKS*NUMCHUNKS];
		Vector2 basesBuff[NUMCHUNKS*NUMCHUNKS];

		double timer = clock();
		
		for(int i = 0; i < NUMCHUNKS; i++) {
			for(int j = 0; j < NUMCHUNKS; j++) {
				basesBuff[i*NUMCHUNKS+j] = Vector2(base.u + i*CHUNKSIZE, base.v + j * CHUNKSIZE);
				if (oldTerrain[basesBuff[i*NUMCHUNKS+j].u].count(basesBuff[i*NUMCHUNKS+j].v) == 0) {
					oldTex = oldTerrain[basesBuff[i*NUMCHUNKS+j].u][basesBuff[i*NUMCHUNKS+j].v];
					std::cout << "Generating New Terrain Chunk: (" << basesBuff[i*NUMCHUNKS+j].u << ", " << basesBuff[i*NUMCHUNKS+j].v << ")" << std::endl;
					timer = clock();
					newTerrain[basesBuff[i*NUMCHUNKS+j].u][basesBuff[i*NUMCHUNKS+j].v] = 
								Texture(Texture::DISPLACEMENT, heights.TBBSIMDGenerateHeightField(basesBuff[i*NUMCHUNKS+j].u, basesBuff[i*NUMCHUNKS+j].v, noise, CHUNKSIZE), 1024);
					timer = clock()-timer;
					std::cout << "Done in " << timer/CLOCKS_PER_SEC << "s" << std::endl;
				} else {
					oldTex = oldTerrain[basesBuff[i*NUMCHUNKS+j].u][basesBuff[i*NUMCHUNKS+j].v];
					newTerrain[basesBuff[i*NUMCHUNKS+j].u][basesBuff[i*NUMCHUNKS+j].v] = oldTex;
				}
				materialMutex.lock_read();
				Material defaultGround = materials[i*NUMCHUNKS+j];
				materialMutex.unlock();
				defaultGround.ReplaceTexture(Texture::DISPLACEMENT, newTerrain[basesBuff[i*NUMCHUNKS+j].u][basesBuff[i*NUMCHUNKS+j].v]);
				defaultWater.ReplaceTexture(Texture::DISPLACEMENT, newTerrain[basesBuff[i*NUMCHUNKS+j].u][basesBuff[i*NUMCHUNKS+j].v]);
				oldTex = newTerrain[basesBuff[i*NUMCHUNKS+j].u][basesBuff[i*NUMCHUNKS+j].v];
				materialsBuff[i*NUMCHUNKS+j] = defaultGround;
				waterMatsBuff[i*NUMCHUNKS+j] = defaultWater;
			}
		}

		terrainMutex.lock();
		newTerrain.swap(*terrainMap);
		terrainMutex.unlock();

		materialMutex.lock();
		waterMutex.lock();
		basesMutex.lock();
		for (int i = 0; i < NUMCHUNKS;i++) {
			for (int j = 0; j < NUMCHUNKS; j++) {
				materials[i*NUMCHUNKS+j] = materialsBuff[i*NUMCHUNKS+j];
				waterMaterials[i*NUMCHUNKS+j] = waterMatsBuff[i*NUMCHUNKS+j];
				bases[i*NUMCHUNKS+j] = basesBuff[i*NUMCHUNKS+j];
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

	tbb::atomic<Vector2*> bases;
	tbb::atomic<Material*> materials;
	tbb::atomic<Material*> waterMaterials;

	Material defaultGround;
	Material defaultWater;
	NoiseObject noise;

//  MUTEXES

	tbb::spin_rw_mutex materialMutex;
	tbb::spin_rw_mutex waterMutex;
	tbb::spin_rw_mutex basesMutex;
	tbb::spin_rw_mutex terrainMutex;

//  ATOMIC COUNTER
	tbb::atomic<int>* threadCounter;
};
