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

	Matrix4x4 BuildModelMatrix(GameObject g);
	Matrix4x4 BuildViewMatrix();
	void BuildProjectionMatrix();
	void BuildModelViewMatrix(GameObject g);
	void BuildSkyBoxViewMatrix(GameObject g);

	void ConvertToOpenGLMatrix(Matrix4x4 m, GLfloat* target);

	std::list<int> renderList;
	std::vector<int> opaqueRenderList;

//	GLfloat modelMatrix[16];
//	GLfloat viewMatrix[16];
	GLfloat modelViewMatrix[16];
	GLfloat projectionMatrix[16];

	GLuint skyBox;
	GLuint skyBoxTexture;
	GLuint terrain;
	GLuint terrainTexture;

	GameObject base;

	std::map<int, GLuint> UniqueIDToDListMap;
	std::map<std::string, GLuint> TextureMap; 
};

