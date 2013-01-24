#pragma once
#include <vector>
#include "myvector3.h"

class Heightmap
{
public:
	Heightmap(int iterations);
	void Initialise();
	~Heightmap(void);
	float GetHeight(int x, int y){return heightmap[x][y];}
	Vector3 GetNormal(int x, int y);
	int GetSize() {return heightmap.size();}
	float GetFloatHeight(float x, float y);
	float GetRelativeFloatHeight(float x, float y);

private:

	void GenHeightmap();
	void GenNormals();
	Vector3 CalcNormal(Vector3, Vector3, Vector3);
	void SquareStep(int xLow, int xHigh, int yLow, int yHigh, int iteration);
	void DiamondStep(int xLow, int xHigh, int yLow, int yHigh, int iteration);

	float GetRandom(float range);

	std::vector<std::vector<float> > heightmap;
	std::vector<std::vector<Vector3> > normalmap;
	const int totalIterations;
	int squareSize;

	static const int SIZE = 256;
	const float roughness;
};

