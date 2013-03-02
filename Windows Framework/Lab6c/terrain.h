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

private:

	float squareSize;
	float resolution;
	float step;
};