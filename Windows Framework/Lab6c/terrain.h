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

	float GetHeight(float x, float z);
	Vector3* GetVertexArrayBase(){return &verts[0];}
	Vector3* GetNormalArrayBase(){return &normals[0];}
	Vector2* GetUVArrayBase(){return &texCoords[0];}
	int GetNumberOfVerts(){return verts.size();}
	std::string GetTexturePath() {return textureFile;}

private:
	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;

	std::string textureFile;

	Vector3 CalcNormal(Vector3 A, Vector3 B, Vector3 C);

	float squareSize;
	float resolution;
};

