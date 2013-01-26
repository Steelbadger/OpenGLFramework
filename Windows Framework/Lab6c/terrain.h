#pragma once
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <gl/gl.h>
#include "tex.h"
#include "myvector3.h"
#include "myvector2.h"
#include "heightmap.h"


class Terrain
{
public:
	Terrain(double size);
	~Terrain(void);

	void Create();
	void CreateFromSource(std::vector<Vector3> &vertices);
	void Draw();

	float GetHeight(float x, float z);
	void SetTexture(GLuint);

	GLuint GetTexture(){return texture;}
	GLuint GetDisplayList(){return displayList;}
	int GetUniqueID(){return UID;}

private:
	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;


	int UID;

	GLuint texture;
	GLuint displayList;

	Heightmap heightmap;

	Vector3 CalcNormal(Vector3 A, Vector3 B, Vector3 C);
	void SmoothNormals();

	double squareSize;

	static int counter;
};

