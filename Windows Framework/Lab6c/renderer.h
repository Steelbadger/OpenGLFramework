#pragma once
#include "glew.h"
#include <vector>
#include <list>
#include <map>
#include <string>
#include <queue>
#include "mesh.h"
#include "terrain.h"
#include "cameramodule.h"
#include "staticobject.h"
#include "mywindow.h"

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
	void SetShaders(std::string vertex, std::string fragment);
	static bool MeshComparator(int rhs, int lhs);

private:

	static CameraModule* activeCamera;

	GLuint CompileToDisplayList(Mesh &m, GLuint texture);
	GLuint CompileToDisplayList(Terrain &t, GLuint texture);
	void InitializeOpenGL();
	void LoadShader(std::string fileName);


	Matrix4x4 BuildModelMatrix(GameObject g);
	Matrix4x4 BuildViewMatrix();
	void BuildProjectionMatrix();
	void BuildModelViewMatrix(GameObject g);
	void BuildSkyBoxViewMatrix(GameObject g);

	void ConvertToOpenGLMatrix(Matrix4x4 m, GLfloat* target);

	std::list<int> renderList;
	std::vector<int> opaqueRenderList;

	GLfloat modelMatrix[16];
	GLfloat viewMatrix[16];
	GLfloat modelViewMatrix[16];
	GLfloat projectionMatrix[16];

	GLuint skyBox;
	GLuint skyBoxTexture;
	GLuint terrain;
	GLuint terrainTexture;

	GameObject base;

	std::map<int, GLuint> UniqueIDToDListMap;
	std::map<int, GLuint> UniqueIDToShaderMap;
	std::map<std::string, GLuint> TextureMap;
	std::map<std::string, GLuint> ShaderMap;

	GLuint defaultShaderProgram;
};

