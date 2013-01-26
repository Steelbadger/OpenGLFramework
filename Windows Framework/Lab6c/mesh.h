#pragma once
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <gl/gl.h>
#include "myvector3.h"
#include "myvector2.h"
#include <string>

class GameObject;


class Mesh
{
public:
	Mesh(const char* meshPath, char* texturePath, GameObject* parent);
	~Mesh(void);

	void Initialise();

	void Draw();

private:
	bool LoadMesh(const char* path);
	bool LoadObj(const char* path);
	void BuildDisplayList();

	bool LoadTexture(char* path);

	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> uvs;

	GLuint dList;
	GLuint texture;

	int triangles;

	GameObject* parent;
	std::string meshPath;
	std::string texturePath;

	bool successfullBuild;
};

