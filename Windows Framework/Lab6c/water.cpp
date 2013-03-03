#include "water.h"


Water::Water(float size):
		squareSize(size)
{
}


Water::~Water(void)
{
}

void Water::Create()
{
	int size = squareSize/10.0;
	float step = squareSize/(size-1);

	for (float i = 0; i < size; i++) {
		for (float j = 0; j < size; j++) {
			verts.push_back(Vector3(i*step, 0, j*step));
			Vector3 normalA = Vector3(0,1,0);
			normals.push_back(normalA);
		}
	}

	for (int i = 0; i < size-1; i++) {
		for (int j = 0; j < size-1; j++) {
			index.push_back(i+j*size);
			index.push_back(1+i+(1+j)*size);
			index.push_back(i+(1+j)*size);
			index.push_back(1+i+(1+j)*size);
			index.push_back(i+j*size);
			index.push_back(1+i+j*size);
		}
	}
}