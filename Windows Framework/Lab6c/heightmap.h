#pragma once
#include <vector>

class Heightmap
{
public:
	Heightmap(int iterations);
	~Heightmap(void);
	float GetHeight(int x, int y){return heightmap[x][y];}
	int GetSize() {return heightmap.size();}

private:

	void GenHeightmap();
	void SquareStep(int xLow, int xHigh, int yLow, int yHigh, int iteration);
	void DiamondStep(int xLow, int xHigh, int yLow, int yHigh, int iteration);

	float GetRandom(float range);

	std::vector<std::vector<float> > heightmap;
	const int totalIterations;
	int squareSize;

	static const int SIZE = 256;
	static const int MAGNITUDE = 200;
};

