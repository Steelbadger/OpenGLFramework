#pragma once
#include <vector>
#include <map>
#include <string>
#include <queue>
#include "Terrain.h"

class RenderManager
{
public:
	RenderManager(void);
	~RenderManager(void);

	int ImportTerrain();

	void AddTerrainToRenderer(Terrain m);
	void RemoveTerrainFromRenderer(Terrain m);

	void RenderTerrain(int TerrainID);
	void RenderAll();

private:

	std::vector<GLuint> renderList;
	std::vector<GLuint> transparentList;
	std::vector<GLuint> inactiveList;

	std::priority_queue<int> renderQueue;

	GLuint skyBox;

	std::map<GLuint, int> renderListMap;
	std::map<GLuint, int> transparentListMap;
	std::map<GLuint, int> inactiveListMap;

	std::map<int, GLuint> UniqueIDToDListMap;

	static int UIDCOUNTER;
};

