#pragma once
#include "mesh.h"

class PrimitiveFactory
{
public:
	PrimitiveFactory(void);
	~PrimitiveFactory(void);

	Mesh Plane(float width, float height, int widthSubDivs, int heightSubDivs);
	Mesh SimpleInnerBox();
};

