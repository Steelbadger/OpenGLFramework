#pragma once
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <gl/gl.h>

class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	void Create();
	void Draw();

private:
	std::vector<float> verts;
	std::vector<float> normals;
};

