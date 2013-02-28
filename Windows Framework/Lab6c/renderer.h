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
#include "lights.h"
#include "material.h"
#include "texture.h"

struct UniformLocations;

class RenderManager
{
public:
	RenderManager();
	~RenderManager(void);

	void MaintainRenderList();

	void SetActiveCamera(CameraModule& cam){activeCamera = &cam;}
	void SetTextureUnitNumber();

	bool AddToRenderer(Mesh &m);
	void RemoveFromRenderer(Mesh m);
	void AddSkyBox(Mesh &m);
	void AddTerrainToRenderer(Terrain &t);
	void BuildDefaultShaderProgram();
	void AddLight(LightSource &l);
	void PassInHeights(GLuint h, float width, float magnitude){heights = h; mapWidth = width; terrainMagnitude = magnitude;}

	void RenderAll();
	void SetShaders(std::vector<std::string> shaders);
	static bool MeshComparator(int rhs, int lhs);

private:

	static CameraModule* activeCamera;

	GLuint SetupVAO(Mesh &m);
	GLuint SetupVAO(Terrain &t);

	void InitializeOpenGL();
	bool LoadShader(std::string fileName);
	GLuint CreateShaderProgram(std::vector<std::string> shaders);


	Matrix4x4 BuildModelMatrix(GameObject g);
	Matrix4x4 BuildViewMatrix();
	void BuildProjectionMatrix();
	void BuildModelViewMatrix(GameObject g);
	void BuildSkyBoxViewMatrix(GameObject g);

	bool viewMatrixMade;
	bool lightsPrepared;

	void PrepareLights();
	void SetUniforms(Material m);
	
	void DrawSkyBox();
	void DrawTerrain();
	bool DrawMesh(int meshID);

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
	GLuint skyBoxTexture;
	Material skyboxMaterial;

	GLuint terrain;
	GLuint terrainTexture;
	GLuint terrainRock;
	GLuint heights;


	int terrainVerts;
	float terrainStep;
	float terrainMagnitude;

	GameObject base;

	LightSource sun;
	Light sunSource;
	std::vector<LightSource> lightObjects;
	Light lights[LightSource::MAXLIGHTS];

	std::map<int, GLuint> VAOMap;
	std::map<int, GLuint> UniqueIDToShaderMap;
	std::map<std::string, GLuint> TextureMap;
	std::map<std::string, GLuint> ShaderMap;
	std::map<std::string, GLuint> MeshFileMap;
	std::map<std::string, GLuint> ShaderProgramMap;
	std::map<GLuint, UniformLocations> ProgramUniformLocationMap;

	GLuint defaultShaderProgram;
	GLuint currentShaderProgram;

	GLuint skyboxShaderProgram;
	GLuint terrainShaderProgram;
	float mapWidth;
	int MaxTextures;
};


struct UniformLocations
{
	GLuint ProjectionMatrix;
	GLuint ViewMatrix;
	GLuint ModelMatrix;
	GLuint ModelViewMatrix;
	GLuint NormalMatrix;
	GLuint LightPositions[LightSource::MAXLIGHTS];
	GLuint LightColours[LightSource::MAXLIGHTS];
	GLuint NumLights;
	GLuint LightTypes[LightSource::MAXLIGHTS];
	GLuint Texture1;
	GLuint Texture2;
	GLuint Texture3;
	GLuint Texture4;
	GLuint MapWidth;
	GLuint Magnitude;
};

