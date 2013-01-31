#pragma once
#include <vector>
#include <list>
#include <map>
#include <string>
#include <queue>
#include "mesh.h"
#include "terrain.h"
#include "cameramodule.h"
#include "staticobject.h"

class RenderManager
{
public:
	RenderManager();
	~RenderManager(void);

	void MaintainRenderList();

	void SetActiveCamera(CameraModule& cam){activeCamera = &cam;}

	bool AddToRenderer(Mesh &m);
	void AddToRenderer(std::vector<Mesh> &meshList);
	void RemoveFromRenderer(Mesh m);
	void AddSkyBox(Mesh &m);
	void AddTerrainToRenderer(Terrain &t);

	void RenderAll();
	void UpdateCamera();

	static bool MeshComparator(int rhs, int lhs);

private:

	static CameraModule* activeCamera;

	GLuint CompileToDisplayList(Mesh &m, GLuint texture);
	GLuint CompileToDisplayList(Terrain &t, GLuint texture);

	std::list<int> renderList;

	GLuint skyBox;
	GLuint skyBoxTexture;
	GLuint terrain;
	GLuint terrainTexture;

	std::map<int, GLuint> UniqueIDToDListMap;
	std::map<std::string, GLuint> TextureMap; 
};

