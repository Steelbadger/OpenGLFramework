#pragma once
#include "noisegenerator.h"


class Heightmap
{
public:
	Heightmap(void);
	~Heightmap(void);

	unsigned int GenerateHeightmap(float x, float y, NoiseObject n);

};

