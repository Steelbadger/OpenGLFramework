#include "terrainmanager.h"
#include "renderer.h"
#include "terrain.h"
#include "gameobject.h"
#include "heightmap.h"
#include "material.h"

#include <iostream>




TerrainManager::TerrainManager(void) : noise(12, 500.0f, 0.41f, 80.0f, -1563.155f),	terrain(chunkSize, 15.0f), water(chunkSize)
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
			bases[i][j] = Vector2(base.u + i*chunkSize, base.v + j * chunkSize);
			std::cout << "Generating at: (" << bases[i][j].u << ", " << bases[i][j].v << ")" << std::endl;
			terrainMap[bases[i][j].u][bases[i][j].v] = 
							Texture(Texture::DISPLACEMENT, heights.TBBGenerateHeightField(bases[i][j].u, bases[i][j].v, noise, chunkSize), 2048);
			std::cout << "Applying Material..." << std::endl;
			defaultGround.ReplaceTexture(oldTex, terrainMap[bases[i][j].u][bases[i][j].v]);
			defaultWater.ReplaceTexture(oldTex, terrainMap[bases[i][j].u][bases[i][j].v]);
			oldTex = terrainMap[bases[i][j].u][bases[i][j].v];
			materials[i][j] = defaultGround;
			waterMats[i][j] = defaultWater;
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
	Heightmap heights;
	std::map<float, std::map<float, Texture> > oldTerrain;
	oldTerrain.swap(terrainMap);
	Texture oldTex;

	for(int i = 0; i < numChunks; i++) {
		for(int j = 0; j < numChunks; j++) {
			bases[i][j] = Vector2(base.u + i*chunkSize, base.v + j * chunkSize);
			if (oldTerrain[bases[i][j].u].count(bases[i][j].v) == 0) {
				oldTex = oldTerrain[bases[i][j].u][bases[i][j].v];
				std::cout << "Generating New Terrain Chunk: (" << bases[i][j].u << ", " << bases[i][j].v << ")" << std::endl;
				terrainMap[bases[i][j].u][bases[i][j].v] = 
							Texture(Texture::DISPLACEMENT, heights.TBBGenerateHeightField(bases[i][j].u, bases[i][j].v, noise, chunkSize), 2048);
			} else {
				oldTex = oldTerrain[bases[i][j].u][bases[i][j].v];
				terrainMap[bases[i][j].u][bases[i][j].v] = oldTerrain[bases[i][j].u][bases[i][j].v];
			}

			Material defaultGround = materials[i][j];
			defaultGround.ReplaceTexture(oldTex, terrainMap[bases[i][j].u][bases[i][j].v]);
			defaultWater.ReplaceTexture(oldTex, terrainMap[bases[i][j].u][bases[i][j].v]);
			oldTex = terrainMap[bases[i][j].u][bases[i][j].v];
			materials[i][j] = defaultGround;
			waterMats[i][j] = defaultWater;
		}
	}
}
