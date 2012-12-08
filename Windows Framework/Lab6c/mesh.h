#pragma once
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <gl/gl.h>
#include "tex.h"

class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	void Create();
	void Draw();

	GLuint texture;

private:
	std::vector<float> verts;
	std::vector<float> normals;
	std::vector<float> texCoords;
};

