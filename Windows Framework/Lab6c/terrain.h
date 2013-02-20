#pragma once
#include <vector>
#include <windows.h>
#include <stdio.h>
#include "tex.h"
#include "myvector3.h"
#include "myvector2.h"

class NoiseObject;

class Terrain
{
public:
	Terrain(float size, NoiseObject n, float resolution);
	~Terrain(void);

	void Create(NoiseObject n);

	Vector3* GetVertexArrayBase(){return &verts[0];}
	Vector3* GetNormalArrayBase(){return &normals[0];}
	Vector2* GetUVArrayBase(){return &texCoords[0];}
	unsigned int* GetIndexArrayBase(){return &index[0];}
	int GetNumberOfVerts(){return verts.size();}
	int GetIndexLength(){return index.size();}
	std::string GetTexturePath() {return textureFile;}
	std::string GetRockTexturePath() {return rockFile;}
	void AttachShader(std::string shader);

	std::string GetVertexShader() {return vertexShader;}
	std::string GetFragmentShader() {return fragmentShader;}
	std::vector<std::string> GetShaders(){return shaders;}

private:
	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;
	std::vector<unsigned int> index;

	std::string textureFile;
	std::string rockFile;

	float baseX;
	float baseY;

	std::string vertexShader;
	std::string fragmentShader;
	std::string tesselationControlShader;
	std::string tesselationEvaluationShader;

	std::vector<std::string>shaders;

	float squareSize;
	float resolution;
};

