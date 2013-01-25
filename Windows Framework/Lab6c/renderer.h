#pragma once
#include <vector>
#include <map>
#include <string>
#include "mesh.h"

class Renderer
{
public:
	Renderer(void);
	~Renderer(void);

	void AddMeshToRenderer(Mesh m);
	void RemoveMeshFromRenderer(Mesh m);

	void RenderMesh(Mesh m);
	void RenderAll();

private:

	std::vector<GLuint> renderList;
	std::vector<GLuint> transparentList;
	std::vector<GLuint> inactiveList;

	GLuint skyBox;

	std::map<GLuint, int> renderListMap;
	std::map<GLuint, int> transparentListMap;
	std::map<GLuint, int> inactiveListMap;

	std::map<int, GLuint> UniqueIDToDListMap;
};

