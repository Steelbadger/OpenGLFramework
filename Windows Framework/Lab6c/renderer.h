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
	void RemoveFromRenderer(Mesh m);
	void AddSkyBox(Mesh &m);
	void AddTerrainToRenderer(Terrain &t);
	void BuildDefaultShaderProgram();

	void RenderAll();
	void SetShaders(std::string vertex, std::string fragment);
	static bool MeshComparator(int rhs, int lhs);

private:

	static CameraModule* activeCamera;

	GLuint SetupVAO(Mesh &m);
	GLuint SetupVAO(Terrain &t);

	void InitializeOpenGL();
	bool LoadShader(std::string fileName);
	GLuint CreateShaderProgram(std::string vertex, std::string fragment);

	Matrix4x4 BuildModelMatrix(GameObject g);
	Matrix4x4 BuildViewMatrix();
	void BuildProjectionMatrix();
	void BuildModelViewMatrix(GameObject g);
	void BuildSkyBoxViewMatrix(GameObject g);
	
	void DrawSkyBox();
	void DrawTerrain();
	bool DrawMesh(int meshID);

	void ConvertToOpenGLMatrix(Matrix4x4 m, GLfloat* target);

	std::list<int> renderList;
	std::vector<int> opaqueRenderList;

	GLfloat modelMatrix[16];
	GLfloat viewMatrix[16];
	GLfloat modelViewMatrix[16];
	GLfloat projectionMatrix[16];
	GLfloat normalMatrix[16];

	GLuint skyBox;
	GLuint skyBoxTexture;

	GLuint terrain;
	GLuint terrainTexture;
	GLuint terrainRock;

	int terrainVerts;

	GameObject base;

	std::map<int, GLuint> VAOMap;
	std::map<int, GLuint> UniqueIDToShaderMap;
	std::map<std::string, GLuint> TextureMap;
	std::map<std::string, GLuint> ShaderMap;
	std::map<std::string, GLuint> MeshFileMap;
	std::map<std::string, GLuint> ShaderProgramMap;

	GLuint defaultShaderProgram;
	GLuint currentShaderProgram;

	GLuint skyboxShaderProgram;
	GLuint terrainShaderProgram;
};

