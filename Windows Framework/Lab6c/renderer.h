#pragma once
#include "cameramodule.h"
#include "defines.h"
#include "glew.h"
#include "lights.h"
#include "material.h"
#include "mesh.h"
#include "mywindow.h"
#include "staticobject.h"
#include "terrain.h"
#include "water.h"

#include <vector>
#include <list>
#include <map>
#include <string>
#include <queue>

class RenderManager
{
public:
	RenderManager();
	~RenderManager(void);

	void MaintainRenderList();

	void SetActiveCamera(CameraModule& cam){activeCamera = &cam;}
	GameObject* GetCameraParent(){return activeCamera->GetParent();}

	bool AddToRenderer(Mesh &m);
	void RemoveFromRenderer(Mesh m);
	void AddSkyBox(Mesh &m);
	void AddTerrainToRenderer(Terrain &t);
	void AddLight(LightSource &l);
	void AddWater(Water &w);

	void RenderAll();
	static bool MeshComparator(int rhs, int lhs);

private:

	static CameraModule* activeCamera;

	GLuint SetupVAO(Mesh &m);

	void InitializeOpenGL();

	Matrix4x4 BuildModelMatrix(GameObject g);
	Matrix4x4 BuildViewMatrix();
	void BuildProjectionMatrix();
	void BuildModelViewMatrix(GameObject g);
	void BuildSkyBoxViewMatrix(GameObject g);

	bool viewMatrixMade;
	bool lightsPrepared;

	void PrepareLights();
	void SetUniforms(UniformLocations l);
	
	void DrawSkyBox();
	void DrawTerrain();
	bool DrawMesh(int meshID);
	void DrawWater();
	float elapsed;

	void ConvertToOpenGLMatrix(Matrix4x4 m, GLfloat* target);

	std::list<int> renderList;
	std::vector<int> opaqueRenderList;

	GLfloat modelMatrix[16];
	GLfloat viewMatrix[16];
	Matrix4x4 viewMat;
	GLfloat modelViewMatrix[16];
	GLfloat projectionMatrix[16];
	GLfloat normalMatrix[16];

	GLuint skyBox;
	int sky;

	GLuint terrain;
	int terr;
	float terrainStep;
	float terrainSize;

	GLuint water;
	int waterID;

	GameObject base;

	LightSource sun;
	Light sunSource;
	std::vector<LightSource> lightObjects;
	Light lights[MAXLIGHTS];

	std::map<int, GLuint> VAOMap;
	std::map<std::string, GLuint> MeshFileMap;
};
