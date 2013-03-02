#pragma once

#include "mesh.h"
#include "noisegenerator.h"

class Terrain : public Mesh
{
public:
	Terrain(float size, NoiseObject n, float resolution);
	~Terrain(void);

	void Create(NoiseObject n);

	float GetStep(){return step;}
	float GetSize(){return squareSize;}

private:

	float squareSize;
	float resolution;
	float step;
};