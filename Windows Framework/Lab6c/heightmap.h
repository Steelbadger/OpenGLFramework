#pragma once
class Heightmap
{
public:
	Heightmap(void);
	~Heightmap(void);
	float GetHeight(int x, int y){return heightmap[x][y];}

private:

	void GenHeightmap();
	float heightmap[50][50];

	float Noise(float x, float y);
	float Grad(int hash, float x, float y);
	const static unsigned char perm[512];

};

