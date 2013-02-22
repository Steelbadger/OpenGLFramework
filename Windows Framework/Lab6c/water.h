#pragma once
#include "myvector3.h"
#include <vector>

class Water
{
public:
	Water(float size, float xBase, float yBase);
	~Water(void);
	void Create();

	Vector3* GetVertexArrayBase(){return &verts[0];}
	Vector3* GetNormalArrayBase(){return &normals[0];}

	float GetBaseX(){return baseX;}
	float GetBaseY(){return baseY;}

	void AttachShader(std::string shader);
	std::vector<std::string> GetShaders(){return shaders;}

private:
	std::vector<Vector3> verts;
	std::vector<Vector3> normals;
	std::vector<unsigned int> index;

	float baseX;
	float baseY;
	float squareSize;

	std::vector<std::string>shaders;

};

