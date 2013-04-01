#include "terrainmanager.h"
#include "renderer.h"
#include "terrain.h"
#include "gameobject.h"




TerrainManager::TerrainManager(void)
{
}


TerrainManager::~TerrainManager(void)
{
}


void TerrainManager::Initialize(RenderManager &r)
{
	renderer = &r;
	camera = renderer->GetCameraParent();


	bases[0][0] = (camera->GetPosition().xz()-range)/chunkSize;
	bases[numChunks-1][numChunks-1] = (camera->GetPosition().xz()+range)/chunkSize;
	bases[0][numChunks-1] = Vector2((camera->GetPosition().x-range), (camera->GetPosition().z+range))/chunkSize;
	bases[numChunks-1][0] = Vector2((camera->GetPosition().x+range), (camera->GetPosition().z-range))/chunkSize;

	for (int i = 0; i < numChunks; i++) {
		for (int j = 0; j < numChunks; j++) {
		bases[i][j].Floor();
		bases[i][j] = bases[i][j] * chunkSize;
		}
	}
}

void Update()