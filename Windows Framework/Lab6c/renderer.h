#pragma once
#include <vector>
#include <map>
#include <string>
#include <queue>
#include "mesh.h"

class RenderManager
{
public:
	RenderManager(void);
	~RenderManager(void);

	int ImportMesh();

	void AddMeshToRenderer(Mesh m);
	void RemoveMeshFromRenderer(Mesh m);

	void RenderMesh(int MeshID);
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

