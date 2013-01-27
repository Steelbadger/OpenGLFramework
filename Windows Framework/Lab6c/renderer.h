#pragma once
#include <vector>
#include <list>
#include <map>
#include <string>
#include <queue>
#include "mesh.h"
#include "cameramodule.h"

class RenderManager
{
public:
	RenderManager();
	~RenderManager(void);

	void MaintainRenderList();

	void SetActiveCamera(CameraModule& cam){activeCamera = &cam;}

	bool AddToRenderer(Mesh m);
	void RemoveFromRenderer(Mesh m);
	void AddSkyBox(Mesh m);

	void RenderAll();

	static bool MeshComparator(int rhs, int lhs);

private:

	static 	CameraModule* activeCamera;

	GLuint CompileToDisplayList(Mesh m, GLuint texture);

	std::list<int> renderList;

	GLuint skyBox;
	GLuint terrain;

	std::map<int, GLuint> UniqueIDToDListMap;
	std::map<std::string, GLuint> TextureMap; 
};

