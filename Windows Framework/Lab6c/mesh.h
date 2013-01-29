#pragma once
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>
#include <gl/gl.h>
#include "myvector3.h"
#include "myvector2.h"
#include <string>
#include <map>

class GameObject;


class Mesh
{
public:
	Mesh(const char* meshPath, const char* texturePath, GameObject* parent);
	Mesh(const char* meshPath, const char* texturePath);
	Mesh();
	Mesh(const Mesh& mesh);
	~Mesh(void);

	void Initialize();

	int GetUniqueID(){return uniqueID;}
	void GetNewUniqueID();
	void SetParent(GameObject* p) {parent = p;}
	GameObject* GetParentPointer(){return parent;}
	bool IsTransparent(){return transparency;}
	void Draw();
	Vector3* GetVertexArrayBase(){return &verts[0];}
	Vector3* GetNormalArrayBase(){return &normals[0];}
	Vector2* GetUVArrayBase(){return &uvs[0];}
	std::string GetTexturePath(){return texturePath;}
	int GetTriangleNumber(){return triangles;}

	static Mesh* GetMeshPointer(int uniqueID);

private:
	bool LoadMesh(const char* path);
	bool LoadObj(const char* path);
	void BuildDisplayList();

	bool LoadTexture(const char* path);

	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<Vector2> uvs;

	GLuint dList;
	GLuint texture;

	int triangles;
	const int uniqueID;

	GameObject* parent;
	std::string meshPath;
	std::string texturePath;

	bool successfullBuild;
	bool transparency;

	static int IDCOUNTER;
	static std::map<int, Mesh*> IdToMeshMap;
};