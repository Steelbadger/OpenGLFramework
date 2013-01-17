#pragma once
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <gl/gl.h>
#include "glext.h"
#include "tex.h"
#include "myvector3.h"
#include "myvector2.h"


class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	void Create();
	void CreateFromSource(std::vector<Vector3> &vertices);
	void Draw();

	GLuint texture;

private:
	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;

	GLuint displayList;
};

